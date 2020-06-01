---
title: "[System] Linux Anti Debugging"
date: 2020-04-27
categories: System
sitemap :
  changefreq : daily
  priority : 1.0
---



---



# 설명

- UNIX 계열의 운영체제에서는 `ptrace` 라는 system call을 통해 프로그램 분석, 수정 작업이 가능하다.
- `ptrace`는 프로세스에 Attach한 뒤 코드, 데이터, 스택, 힙, 레지스터 등에 접근이 가능하다.
- 하나의 process에 Attach할 수 있는 다른 프로세스는 오직 1개 이하이다.
- gdb, strace, ltrace 등 디버깅을 위한 도구들은 `ptrace`를 사용한다.
- 그러한 이유로, 프로그램 실행시 `ptrace`를 자기 자신한테 붙이면 `Anti-Debugging`의 효과를 낸다.
- 참고로 `PTRACE_TRACEME`라고 하는 매크로가 있는데 이를 `ptrace`에 넘겨주면 자신을 Attach 한다.



---

# 실습

```c
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>

void check(void) __attribute__((constructor));

void check(void)
{
        if(ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
                printf("Debugging Detected\n");
                _exit(-1);
        }
}

int main(void)
{
        printf("do stuff outside GDB\n");
        return 0;
}

```

gdb 등 디버깅이 감지되면 특정 문자열 출력과 함께 프로그램이 종료되도록 만들었다.<br/><br/>





```bash
minibeef@argos-edu:~/test$ ls
0427_antigdb  0427_antigdb.c
minibeef@argos-edu:~/test$ ./0427_antigdb 
do stuff outside GDB
```

그냥 실행시키면 괜찮다. gdb로 main에 브포를 걸고 run 시켜 보았다.<br/><br/>





```c
(gdb) b* main
Breakpoint 1 at 0x72b
(gdb) r
Starting program: /home/minibeef/test/0427_antigdb 
Debugging Detected
[Inferior 1 (process 9970) exited with code 0377]
(gdb)
```

`Debugging Detected`라고 나오면서 프로세스가 죽어버린다.<br/><br/>




<br/><br/>
끝.
