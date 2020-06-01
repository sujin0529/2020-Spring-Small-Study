---
title: "[Android] APK 분석 환경 구축"
date: 2020-03-13
categories: Android
sitemap :
  changefreq : daily
  priority : 1.0
---



# 분석의 방법

`APK`란 **Android Application Package**의 약어로, 안드로이드 OS의 소프트웨어를 지칭한다. 보통 메타데이터 역할을 하는 AnroidManifest와 JAVA 언어로 작성된 소스코드가 패키지 형태로 들어있으며 이들이 한대 묶여 압축 파일의 형태를 하고 있다 *(+요즘은 성능의 이점 때문에 C로 이루어진 Shared Object도 함께 쓴다)* 런타임에는 Dalvik이라고 하는 **자바 가상 머신(Java Virtual Machine)**에 JAVA class 파일들이 DEX 형태로 변환되어 올라간다.

  ![image](https://user-images.githubusercontent.com/44183111/76522506-09366880-64aa-11ea-8ec3-8ed3b70b3b3d.png)



이 과정에서 `후킹(hooking)`을 통해 메서드의 흐름을 원하는대로 조작하여 APK에 존재하는 취약점을 점검할 것이다. 후킹이라는 단어를 들으면 가장 먼저 갈고리를 의미하는 *Hook*이 생각나는데, 여기서의 후킹의 어원이 Hook에서 온 것이다(정상적인 프로그램 로직을 낚아채기 때문이다) 때문에 분석 환경을 구축하는 데에는 다음과 같은 준비물이 필요하다.

1. 안드로이드 에뮬레이터 : Nox
2. ADB(Android Debug Bridge)
3. frida, frida-server, frida-tools : 후킹 도구



---



# 1) 안드로이드 에뮬레이터 Nox 설치

 NoxPlayer 공식 홈페이지 접속( https://www.bignox.com/ )

 중앙에 다운로드

  ![image](https://user-images.githubusercontent.com/44183111/76523394-b52c8380-64ab-11ea-8fb3-963244122799.png)



 안내에 따라 설치 후 루팅

  ![image](https://user-images.githubusercontent.com/44183111/76523534-eb6a0300-64ab-11ea-823c-6d522af3399b.png)

  ![image](https://user-images.githubusercontent.com/44183111/76523595-02105a00-64ac-11ea-9f0b-39914f6ffc6c.png)

  
  되도록이면 리얼 머신으로 분석하는 편이 좋으나 NoxPlayer와 같은 에뮬레이터를 사용하면 APK 추출과 루팅 온오프가 편리하기 때문에 고객사가 있는 상황이 아니라면 그냥 에뮬레이터를 설치해서 쓰자.



---



# 2) ADB 설치

 Android SDK Platform Tools 페이지( https://developer.android.com/studio/releases/platform-tools )로 가서 SDK를 받는다. SDK는 Software Development Kit의 약자로, 개발에 필요한 도구들을 모아놓은 것인데 Android SDK 같은 경우는 ADB를 포함하여 여러 유용한 것들이 들어있기 때문에 설치해준다.

  ![image](https://user-images.githubusercontent.com/44183111/76524199-fe310780-64ac-11ea-8036-8d21460eab7f.png)



 다운받은 파일을 압축 해제하면 아래 사진과 같이 파일들이 나올텐데, 본인이 기억할 수 있는 경로에 가져다 놓는다. 왠만하면 지금 경로를 복사해 놓아라

   ![image-20200312220345248](https://user-images.githubusercontent.com/44183111/76593451-b1dfd900-6539-11ea-9295-e5ff9191795d.png)


 
 윈도우 검색으로 **시스템 환경 변수 편집**을 검색하여 들어간다.

   ![image-20200312220453547](https://user-images.githubusercontent.com/44183111/76593502-d471f200-6539-11ea-8b85-4c8fdef48630.png)




 시스템 변수에 아까 복사한 경로를 추가한다(사진 참고)

   ![image](https://user-images.githubusercontent.com/44183111/76524686-d1c9bb00-64ad-11ea-8f7a-16bea378ee16.png)

   ![image](https://user-images.githubusercontent.com/44183111/76593558-03886380-653a-11ea-8c0b-e988e84d5459.png)


   ![image](https://user-images.githubusercontent.com/44183111/76524824-19504700-64ae-11ea-91a0-77161854507a.png)



 [윈도우 + R], cmd에서 adb 명령을 쳐서 잘 설치되었는지 확인한다. 뭐가 잔뜩 나오면 잘 설치된 것이다.

   ![image-20200312221019869](https://user-images.githubusercontent.com/44183111/76593645-3c283d00-653a-11ea-9b25-6dd7ac15b3ec.png)


---



# 3) frida-server, frida, frida-tools 설치

(파이썬 3와 pip가 설치되어 있어야 한다.)

frida는 분석에서 핵심이 될 친구이다. 후킹을 도와주는 도구라고 생각하면 된다. frida의 경우 클라이언트(에뮬레이터를 실행시키는 PC)와 서버(에뮬레이터)가 상호작용하면서 돌아가기 때문에 에뮬레이터 내부에도 frida를 설치해주어야 한다. frida-agent( https://github.com/frida/frida/releases )



아마 NoxPlayer를 이용한다면 x86_64가 맞을 것 이다. (리얼 머신으로 한다면 arm)

  ![image-20200312221851377](https://user-images.githubusercontent.com/44183111/76593678-56fab180-653a-11ea-9248-06396e8d1ea2.png)



이제 Nox를 켜고 adb로 connect 해주자. 로컬 호스트에 62001 포트로 바인딩 해주면 된다.

  ![image-20200313143943000](https://user-images.githubusercontent.com/44183111/76593682-582bde80-653a-11ea-9f32-f183f64dbf23.png)




ADB 를 통해 frida-server 파일을 push 해주자.

```
$ adb push <frida-server> '/data/local/tmp/'
```

frida server에 실행 권한을 주고 백그라운드로 실행시키자.

```
$ adb shell 'chmod +x /data/local/tmp/frida-server;/data/local/tmp/frida-server &'
```


이제 pip를 이용해 frida와 frida-tools를 설치하자. python이 설치되어 있어야 한다.

```
$ pip3 install frida
$ pip3 install frida-tools
```

다음과 같이 나온다면 설치가 완료된 것이다.
```
$ adb connect 127.0.0.1:62001
$ frida-ps -U

 PID  Name
----  ------------------------------------
1458  adbd
2012  android.process.acore
2280  android.process.media
1904  com.android.inputservice
2537  com.android.keychain
2611  com.android.managedprovisioning
2638  com.android.onetimeinitializer
1940  com.android.phone
2552  com.android.providers.calendar
3349  com.android.settings
1848  com.android.systemui
2661  com.android.vending
3645  com.android.vending:download_service
3176  com.facebook.lite
2426  com.facebook.lite:fbns
3278  com.facebook.lite:fwkstartlog
5138  com.google.android.gms
3925  com.google.android.gms.persistent
3225  com.google.android.gms.ui
2972  com.google.android.gms.unstable
2832  com.google.android.play.games
2154  com.google.process.gapps
1966  com.vphone.launcher
5065  debuggerd
2133  dhcpcd
1475  drmserver
5177  frida-server-x86
1457  healthd
   1  init
1477  installd
1478  keystore
1459  lmkd
5179  logcat
1456  logd
1476  mediaserver
1467  netd
1474  rild
1403  sdcard
1460  servicemanager
2711  su
1462  surfaceflinger
1736  system_server
1009  ueventd
1461  vold
2019  wpa_supplicant
1480  zygote
```
