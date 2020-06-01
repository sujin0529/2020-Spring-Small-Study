저번에 만들던 리액트 홈페이지의 css 디자인을 마무리했다.

이 과정에서 새로 알게된 사실이 있어서 간단히 정리하고자 한다. 위의 프로젝트는 리액트로 진행했지만, 일반 웹 프로그래밍을 할 때 css를 처리하는 방법과 비슷해서 위의 카테고리에 글을 올린다.

 

항상 스크롤 바를 사용할 때마다 디자인이 아쉬웠는데, 디자인을 따로 설정해주는 방법이 있더라.

위의 사진처럼 스크롤바의 디자인을 적용하고 싶다면 아래의 코드를 적용하면 될 것 같다. 

 


::-webkit-scrollbar{width: 16px;}
::-webkit-scrollbar-track{background-color: #f6e3e9;}
::-webkit-scrollbar-thumb{background-color: #e09092; border-radius: 10px;}
::-webkit-scrollbar-button:start:decrement,::-webkit-scrollbar-button:end:increment {
    width: 10px; 
}
 
 
위의 코드는 위에서 부터 차례로

스크롤바의 넓이/ 트랙 배경 색상/ 바 색상과 바의 둥근 모서리 설정 정도/ 버튼 넓이 설정을 말한다.

 

간단한 코드이지만 이렇게 사용해보니 훨씬 분위기가 달라진 것을 볼 수 있다. 위 코드는 css 파일 내 아무곳이나 사용해주면 된다. vs code나 다른 코드 편집기를 사용한다면 자동완성이 뜨니 참고하자.
