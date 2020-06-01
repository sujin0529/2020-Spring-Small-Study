오늘은 프로그래밍 언어개론의 9장 polymorphic Subprograms에 대하여 학습하였다.

이전 내용과는 조금 익숙하면서도 어려운 내용이여서 몇번이고 강의를 다시 돌려보았다. 공부한 목차는 아래와 같다.

 

1. Overloaded subprogram(중복 부프로그램)
2. Generic subprogram(포괄형 부프로그램)
3. parametric polyism(인자 다형성)
이에 대해 공부한 내용을 간단히 정리해보겠다.

 

Overloaded subprogram
: 한 참조환경에서 이름이 같은 여러개의 서브 프로그램이다. 각 서브 프로그램은 고유한 protocol을 갖는다.

 

호출 시, Actual parameter의 개수 및 type들과 protocol이 일치하는 서브 프로그램들 중 하나가 호출된다. 일치하는 것이 없으면 오류로 판단하고 오류 메세지를 출력한다.

지원 언어는 C++, Java, Ada, C#과 같은 객체지향 언어들이 대부분이다.

 

Generic Subprogram
: polymorphic Subprogram(다형 부프로그램)이라고도 한다. 여러가지 type의 actual parameter를 전달받아서 실행할 수 있다. 그러나, 이러한 것들을 각각 작성하는 것이 아니라 통합하여 한 개의 함수만을 작성하여 소프트웨어의 재사용성을 향상시키도록 한다.

 

Polymorphic Subprogram에는 두 가지 종류가 있다.

1. Ad hoc polymorphism(특이 다형성)
: 프로토콜은 다르나, 이름이 같은 여러 서브프로그램이 있음. 

각 서브프로그램은 독립적이며, 특정 목적을 가지고 이 프로그램들이 꼭 유사하게 동작할 필요는 없다.

 

2. Parametric polymorphism(인자 다형성)
: Type 이름을 parameter화 한 것이다. 형태는 아래와 같다.


특정 type의 파라미터를 다루는 서브프로그램이 필요할 때, 제네릭 타입 이름을 원하는 type이름으로 지정할 수 있다. 

예시는 아래와 같다.


 

* C의 경우
C언어는 제네릭 기능을 지원하지 않지만, 비슷한 기능으로 매크로를 사용해서 표현할 수 있다.


이렇게 사용하면 크기 비교가 가능한 모든 타입에 대해서 사용이 가능하지만, 아래와 같은 문제점이 발생한다.

1. 타입 검사를 하지 않는다.

2. 다음 값이 body에 나타날 때마다 항상 값을 계산해야 한다.

3. Actual parameter가 side-effect를 가지면 결과를 예측하기 어렵다. (신뢰성 하락)

이러한 문제때문에 잘 쓰지 않는 것이 좋다.
