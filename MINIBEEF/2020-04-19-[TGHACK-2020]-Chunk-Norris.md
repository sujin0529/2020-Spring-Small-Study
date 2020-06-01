---
title: "[TG:HACK 2020] Chunk Norris"
date: 2020-04-19
categories: Write-up
sitemap :
  changefreq : daily
  priority : 1.0
---



`319 pts`, `Reversing` APK 분석 문제였다.

---



# 바이너리 행동 분석

![image](https://user-images.githubusercontent.com/44183111/79644014-22d17b00-81e1-11ea-9cda-987ae4d06257.png)

 문제는 `APK`파일 하나를 던져준다. 이를 에뮬레이터에 올려봤더니 게임으로 보이는 프로그램이 실행되었다.


![image](https://user-images.githubusercontent.com/44183111/79644777-13a0fc00-81e6-11ea-8c57-84e045ffb20b.png)

화면을 클릭하면 총알이 나가고 총알에 맞은 적들이 죽는 형식의 게임인데, 플레이어가 좌우로 움직일 수 없어서 사실상 클리어는 불가능한 게임이다. 이제, 디컴파일 해보자.

---

# Dex 분석

**Decompile**에는 Dex 분석을 용이하게 해주는 `JADX`를 사용했다. 모든 코드를 살펴보기는 어렵기 때문에 특정 문자열로 검색하여 부분적으로만 분석했다. 우선, 게임을 클리어하면 플래그를 얻을 수 있을 것 같아 클리어와 관련된 `wave`라는 문자열을 검색해봤다.



![image](https://user-images.githubusercontent.com/44183111/79644197-4943e600-81e2-11ea-861a-51998807c5db.png)

역시나 `wave`를 다루는 코드가 있었으며 다행히 난독화는 되어있지 않았다.



![image](https://user-images.githubusercontent.com/44183111/79644242-932ccc00-81e2-11ea-89a5-126ebee93e05.png)

해당 코드로 이동하여 분석한 결과 맵에 배치된 몬스터의 수(`Invader.Companion.getNumberOfInvaders()`)가 0이 되었을 때 **단계가 넘어가는** 듯한 로직(`this.waves++`)을 찾을 수 있었다. 헌데, `waves` 가 1000이 되었을 떄 무언가 특이한 흐름을 보여준다. `decrypt`라는 곳에 `Base64`처럼 생긴 꼴과 위 사진에서는 보이지 않지만 `magic` 변수를 파라메터로 넘겨준다.



![image](https://user-images.githubusercontent.com/44183111/79644366-7c3aa980-81e3-11ea-9350-955e937d5a40.png)

`decrypt` 메서드의 구현을 살펴보면 어떤 암호화된 문자열을 인자로 받아온 3개의 데이터에 기반하여 복호화를 하는 것을 볼 수 있다. 추측컨데, 복호화된 문자열이 `플래그`가 될 것이다.



![image](https://user-images.githubusercontent.com/44183111/79644426-e05d6d80-81e3-11ea-81b3-f8c79b6fd3fc.png)

다시 이전 코드로 돌아와서, 그렇다면 어떻게 복호화된 문자열을 얻어낼 것인가에 대해 생각해야 한다. 사실 위의 `decrypt` 메서드의 로직을 역으로 따라가서 만들어 낼 수도 있겠지만 메서드를 **후킹**해서 게임을 강제로 클리어해버리면 더욱 쉽게 꺨 수 있다. 그래서 나는 이 문제를 `frida` 스크립트를 짜서 풀기로 했다. 실제로 익스 코드도 몇줄 안된다.


---

# exploit

```javascript
Java.perform(function() {
    var DecryptClass = Java.use('no.tghack.gaiainvaders.EncryptionKt');
    var PassTheGameClass = Java.use('no.tghack.gaiainvaders.Invader$Companion');
    PassTheGameClass.getNumberOfInvaders.implementation = function() {
        console.log('hook!');
        return 0;
    }

    DecryptClass.decrypt.implementation = function(arg1, arg2, arg3) {
        var ret = this.decrypt(arg1, arg2, arg3);
        console.log('[*] FLAG : ' + ret);
        return ret
    }
});
```

```bash
C:\Users\miny7\Desktop\Reversing-Chunk-Norris-319>frida -U -l exploit.js no.tghack.gaiainvaders
     ____
    / _  |   Frida 12.8.5 - A world-class dynamic instrumentation toolkit
   | (_| |
    > _  |   Commands:
   /_/ |_|       help      -> Displays the help system
   . . . .       object?   -> Display information about 'object'
   . . . .       exit/quit -> Exit
   . . . .
   . . . .   More info at https://www.frida.re/docs/home/

[Samsung SM-G930K::no.tghack.gaiainvaders]-> hook!
[*] FLAG : TG20{Norway, the personal space capital of the world}
```

![image](https://user-images.githubusercontent.com/44183111/79644598-f15aae80-81e4-11ea-8f4f-182e28f3620e.png)



끝
