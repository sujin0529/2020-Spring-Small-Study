---
title: "[Android] APK 동적 분석을 위한 IDA 설정"
date: 2020-04-23
categories: Android
sitemap :
  changefreq : daily
  priority : 1.0
---



---



# 개괄

 최근 모 어플 분석을 통해 `보안 솔루션 우회`에 대해 공부하던 중, Dex 분석과 같은 정적인 방법으로는 해결할 수 없는 케이스가 생겨서 IDA를 이용한 원격 분석을 하기 위한 설정법에 대해 글을 써보고자 한다. 정적 분석으로는 한계가 있는 상황은 여러가지가 있는데 나의 경우 `App Sealing`이라고 하는 중국계 보안 솔루션이 JNI를 메모리에 불러올 때, 기존에 암호화되어있던 `Shared Object`를 보안 모듈에서 복호화한 후 올려버려서 단순히 APK를 Unpacking하고 리소스를 분석해서 후킹 포인트를 잡는데는 무리가 있었다.

 이 경우 정적 분석으로는 도저히 암호화된 `Shared Object`를 파악할 수가 없는데, 실제로 APK를 실행하고 메모리에 올라오는 라이브러리를 동적 분석하게 되면 이 어려움이 어느정도 해결이 된다. 아래는 내가 겪었던 케이스들 중 하나이다.



![KakaoTalk_20200423_154551395](https://user-images.githubusercontent.com/44183111/80098036-2ed58800-85a7-11ea-909c-05ed75cd740b.png)

 `App Sealing`이 적용된 APK가 흔하지 않아서 여기저기 찾아보다가 어떤 게임에 해당 기술이 적용 되었다는 칼럼을 보고 바로 분석에 돌입했다. 사진을 보면 `libcovault-appsec.so`는 App Sealing을 적용하면 자동으로 생성되는 보안 모듈이며 여기서 에뮬, 루팅 탐지는 물론 메모리에 올라가는 Dex의 암호화와 안티 리버싱까지 지원하는 무지막지한 놈이다. 헌데, 위 5개의 모듈을 분석해 봤지만 실질적으로 게임 내의 로직에 영향을 미치는 라이브러리가 없었다. 이 게임의 경우 `MONO 엔진`이라고 하는 게임 엔진을 사용하기 때문에 당연하게 `libmono.so`라고 하는 라이브러리가 있어야 한다. 분석 초기에는 굉장히 의아했지만 메모리 이곳 저곳을 후킹하면서 얻어낸 정보에 의하면 `libmono.so`는 어딘가 숨어있다가 메모리에 띠용 하고 올라온다.



![KakaoTalk_20200423_154145463](https://user-images.githubusercontent.com/44183111/80098488-f1bdc580-85a7-11ea-991d-83169727771e.png)

안드로이드는 기본적으로 동적 라이브러리를 불러올 때 `dlopen`이라고 하는 네이티브 함수를 호출하게 되는데, frida로 `Shared Object` 분석을 할때 해당 함수에 후킹을 걸면 메모리에 올라가고 내려가는 형태를 쉽게 캐치할 수 있다. 위 사진을 보면 아까는 전~혀 보이지 않았던 `libmono.so`가 메모리에 올라오는 것을 알 수 있다. 이러한 케이스 말고도 동적 분석이 필요한 경우가 점점 많아질 것 같기 때문에 IDA를 이용한 원격 분석 환경을 구축해보려고 한다..



---



# IDA 설치

IDA에 원격으로 에뮬을 붙일 것이기 때문에 당연히 IDA가 필요하다.



![image](https://user-images.githubusercontent.com/44183111/80098932-a5bf5080-85a8-11ea-8461-17576cb14689.png)

*리버싱 짱 잘하는 누나 등장*



![image](https://user-images.githubusercontent.com/44183111/80099507-8543c600-85a9-11ea-8422-f05d76efea2e.png)

잘 동작하는지 아무거나 올려봤는데 IDA가 PE 분석까지 지원하는지는 처음알았다..ㅋㅋ 윈도우 분석할 때 인터페이스 구닥다리 Oliydbg같은거 이제 안쓴다.



---



# ADB 설치

<a href=" https://developer.android.com/studio/releases/platform-tools ">SDK 설치</a>

![image](https://user-images.githubusercontent.com/44183111/80099709-e10e4f00-85a9-11ea-9dac-556d687e29f0.png)



---



# android_server를 분석 대상에 push하기

IDA Pro 설치 폴더로 이동하면 dbgsrv라는 폴더에 `android_server`라는 파일이 있다. 얘가 나중에 IDA와 원격 통신을 할 때 서버 역할을 하고, IDA가 클라이언트가 되어서 설정된 포트로 붙을 수 있도록 한다.



![image](https://user-images.githubusercontent.com/44183111/80100048-5da12d80-85aa-11ea-9092-a7eb3d000d1a.png)

*별 색칠 잘 됐다..헤헤*

이제 얘를 ADB로 Push 해줄건데 당연히 에뮬이나 리얼 머신에 attach가 되어있는 상태여야 한다.



![image](https://user-images.githubusercontent.com/44183111/80100501-ffc11580-85aa-11ea-9158-dee2e8f813da.png)

 attach 해주고 경로는 `/data/local/tmp/`로 넣어주자



![image](https://user-images.githubusercontent.com/44183111/80102860-59770f00-85ae-11ea-8ed4-bcd2704a47d5.png)

 퍼미션 주고 실행시키면 23946으로 listen하게 된다. 이제 다른 터미널을 띄우자



![image](https://user-images.githubusercontent.com/44183111/80100944-aad1cf00-85ab-11ea-9f18-217f10edd516.png)

포트를 23946으로 땡겨준다.



---



# IDA에서 접속하기

![image](https://user-images.githubusercontent.com/44183111/80101100-e40a3f00-85ab-11ea-9dfd-397ce20e3cfd.png)

`netcfg`로 wlan0 주소를 파악해준다. 요건 사람마다 다를거다 나는 `172.17.99.15`



![image](https://user-images.githubusercontent.com/44183111/80101274-26338080-85ac-11ea-9e6e-afda205f7b44.png)

IDA에서 **Debugger > Attach > Remote ARMLinux/Android Debugger**



![image](https://user-images.githubusercontent.com/44183111/80101446-5f6bf080-85ac-11ea-842a-0251c3f2901d.png)

wlan 주소 입력, 포트는 자동으로 입력되는 것 같은데 안된다면 23946으로 잡아주도록 하자. (참고 : Nox, LD와 같은 에뮬레이터 - 즉, USB를 통해 연결한 리얼 머신이 아닌 내 컴퓨터에서 VT를 이용해 띄운 머신의 경우 localhost를 잡아야 한다.. ㅋㅋ 삽질 10분했다)



![image](https://user-images.githubusercontent.com/44183111/80101765-d1443a00-85ac-11ea-98e6-c5378cdbc3c3.png)

![image](https://user-images.githubusercontent.com/44183111/80101712-bffb2d80-85ac-11ea-8d65-e10cbd511acd.png)

제대로 붙으면 위와같이 나온다. 이제 본인이 분석하고자 하는 프로세스를 잡아서 분석하면 된다.



![image](https://user-images.githubusercontent.com/44183111/80103044-9216e880-85ae-11ea-9084-c67e6630a3d3.png)

이제 분석 대상 APK를 동적으로 쓱싹 조져주면 된다..ㅎㅎ



![image](https://user-images.githubusercontent.com/44183111/80102219-6b0be700-85ad-11ea-956a-86b57595cff5.png)

개괄에서 말했던 것처럼 이렇게 동적으로 붙으면 메모리에 올라온 모듈들을 볼 수 있다. 



![image](https://user-images.githubusercontent.com/44183111/80102391-9db5df80-85ad-11ea-98cb-8f8c0abba826.png)

인스트럭션도 볼 수 있다.. 세상에나,, 물론 동적으로 붙은 상황에서는 Hex-Ray가 지원 안하는 것 같다. 그래도.. 몇일동안 막혀있던 문제를 해결할 수 있었다. 진짜 IDA Pro 만든사람 노벨 평화상 줘라



끝
