---
title: "[System] Function Calling Convention"
date: 2020-03-27
categories: System
sitemap :
  changefreq : daily
  priority : 1.0
---

`[reference]` : <a href="https://docs.microsoft.com/ko-kr/cpp/cpp/argument-passing-and-naming-conventions?view=vs-2019">Microsoft 기술문서</a>, <a href="[https://ko.wikipedia.org/wiki/%ED%98%B8%EC%B6%9C_%EA%B7%9C%EC%95%BD](https://ko.wikipedia.org/wiki/호출_규약)">Wiki</a>, <a href="https://en.wikibooks.org/wiki/X86_Disassembly/Calling_Conventions">Wiki Books</a>

---

# 함수 호출 규약이란

 이 글에서 다루고자 하는 `함수 호출 규약(Function Calling Convention)`이란, **서브루틴**을 호출할 때 그들이 **caller**로부터 **arguments**를 전달 받는 방식과 서브루틴 종료시 돌려주는 **return**의 형태를 일컫는다. 보통 함수 호출 규약은 CPU의 아키텍처와 운영체제에 따라 여러 종류(x86, MIPS, ARM, ...)로 나뉘는데, 여기서는 **x86**에 대해서만 공부해보려 한다. `x86 Calling Convention`은 대표적으로 `stdcall`, `cdecl`, `fastcall` 세가지 방식이 존재한다(이외의 것은 다루지 않는다. - Microsoft 기술 문서 참고).

---

# __stdcall

| **arguments 전달 순서** | **가장 오른쪽 인자부터 전달한다. (**←**)**              |
| ----------------------- | ------------------------------------------------------- |
| **arguments 전달 방법** | Stack 을 사용한다.                                      |
| **Stack Frame 정리**    | 호출을 당한 Callee가 함수를 종료하면서 인자를 정리한다. |

 경우에 따라 `WINAPI`라고도 불리는 것 같다. 그 이유는 이 규약이 Microsoft에서 **Win32 API 표준 호출 규약**으로 거의 독점적으로 사용하고 있기 때문이다.

- `__stdcall`은 오른쪽에서 왼쪽으로(←) **arguments**를 전달한다. 서브 루틴이 종료될 때 **eax**레지스터로 반환 값을 돌려준다.
- 서브 루틴이 종료되기 직전에 callee가 **Call Stack**을 정리한다. 이는 `__stdcall`이 가변 길이 인자(...으로 명시하여 사용하는)를 허용하지 않는다는 것을 말해준다.



```c
__stdcall int stdcall_func(int a, int b) { return a + b; }
int main() { 
    stdcall_func(2, 3); 
}
```

 실제 `__stdcall`이 어떻게 동작하는지 보기 위해 간단한 c 코드를 가져왔다. 인자로 들어온 두 값을 더하는 함수이다. 이를 디스어셈블하면 다음과 같다.



```c
:stdcall_func
push ebp
mov ebp, esp
mov eax, [ebp + 0x8]
mov edx, [ebp + 0xc]
add eax, edx
pop ebp
ret 8	;arrange arguments

:main
push 3	;passing arguments right to left
push 2
call stdcall_func
```
---


# __cdecl

| **arguments 전달 순서** | **가장 오른쪽 인자부터 전달한다. (**←**)** |
| ----------------------- | ------------------------------------------ |
| **arguments 전달 방법** | Stack 을 사용한다.                         |
| **Stack Frame 정리**    | 함수를 호출한 caller가 인자를 정리한다.    |

`__cdecl`은 C와 C++에서 기본 호출 규약이다. 아마도 **C Declaraction**의 약어일거다. `__stdcall`과 거의 비슷한 양상을 가지고 있지만 서브 루틴 종료시 Call Stack의 정리를 caller가 하기 때문에 가변 인자 길이를 허용한다. 

- `__cdecl`은 오른쪽에서 왼쪽으로(←) **arguments**를 전달한다. 서브 루틴이 종료될 때 **eax**레지스터로 반환 값을 돌려준다.
- **caller**가 **Call Stack**을 정리한다. 이러한 이유로 가변 인자 길이를 사용할 수 있지만, 컴파일러가 서브 루틴에 인자의 개수를 고려하지 않게 되므로 어셈블러와 링커가 올바르지 않은 수의 인자 개수를 파악할 수 없다.

```c
__cdecl int cdecl_func(int a, int b) { return a + b; }
int main() { 
    cdecl_func(2, 3); 
}
```

```c
:cdecl_func
push ebp
mov ebp, esp
mov eax, [ebp + 0x8]
mov edx, [ebp + 0xc]
add eax, edx
pop ebp
ret

:main
push 3	;passing arguments right to left
push 2
call cdecl_func
add esp, 8	;arrange arguments
```


---
# __fastcall

| **arguments 전달 순서** | **가장 오른쪽 인자부터 전달한다. (**←**)** |
| ----------------------- | ------------------------------------------ |
| **arguments 전달 방법** | ECX, EDX, Stack                            |
| **Stack Frame 정리**    | Callee가 인자를 정리                       |

`__fastcall`은 이름부터 빨라 보인다👍. 인자는 두개까지는 레지스터에 저장하고 나머지는 스택에 넣는다. 내가 알고있는 `__fastcall`은 ESI, EDI, ECX, EDX, R7, R8, .. 이렇게 인자를 전달하는 것이었는데 뭔가 좀 다르다. 이부분은 조금 더 공부 필요

- 레지스터를 사용하기 때문에 빠르다. 하지만 모든 환경에서 지원하는 것은 아니기 때문에 불안정하다고 한다.
- 인자가 두개를 넘어가면 그 순간부터 Stack을 쓰게 된다. 이렇게 되면 인자를 정리해야 하는데 이는 callee가 하게된다.

```c
__fastcall int fastcall_func(int a, int b) { return a + b; }
int main() { 
    fastcall_func(2, 3); 
}
```

```c
:fastcall_func
push ebp
mov ebp, esp
add eax, edx	;a is in eax, b is in edx
pop ebp
ret

:main
mov ecx, 2
mov edx, 3
call fastcall_func
```



