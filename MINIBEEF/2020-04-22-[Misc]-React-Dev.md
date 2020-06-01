---
title: "[Misc] React 연습(20200422)"
date: 2020-04-22
categories: Misc
sitemap :
  changefreq : daily
  priority : 1.0
---



 <a href="https://github.com/Homeless-ARGOS/MINIBEEF_project">레퍼지토리</a>



# 개괄

![image](https://user-images.githubusercontent.com/44183111/79988769-fb80f380-84e9-11ea-9f8c-a211b2e802e4.png)

`ARGOS` 홈페이지를 리뉴얼하기 위해 프론트엔드 부터 차근차근 공부하는 중이다. `웹 어플리케이션`에서 그치는 것이 아니라 `React-Native`를 활용한 **크로스 플랫폼** 서비스를 개발하는 것이 최종 목표이기 때문에 프론트엔드의 경우 `React`를 선택했다. `React`는 Facebook의 JSX를 기반하고 있으며 사용자 측면에서는 적은 랜더링과 그에 따른 압도적인 성능이, 개발자 측면에서는 컴포넌트를 마치 레고 블럭 끼우듯이 진행되는 워크 플로우가 강한 장점이 되는 라이브러리이다. 

오늘 공부는 `React`개발 감을 잃지 않기 위해 아기자기하게 아르고스 홈페이지를 만들어봤다. 대략 1시간동안 만든거라 퀄리티가 떨어질 뿐만 아니라 실제 적용될 모델은 더욱 더 아니다.



---



# Welcome 페이지

### 디자인

![image](https://user-images.githubusercontent.com/44183111/79988825-118eb400-84ea-11ea-88a9-380b158b55d7.png)

모든 페이지에서 상단의 `아르고스 로고`와 `네비게이션 바`는 고정이다. URL에 접근했을 때 가장 먼저 보이는 페이지가 될 `Welcome`은 유저들의 글 목록을 보여준다. **글쓰기**를 통해 글을 등록할 수도 있다.



### 구현

```react
import React, { Component } from 'react';
import './ArticleList.css'

class ArticleList extends Component {
    render() {
        return (
            <div id="article-list">
                <table width="70%" cellSpacing="0" align="center" border-top="1px solid #444444" border-collapse="collapse">
                    {/* @thead : 제목 작성자 날짜 표시 */}
                    <thead>
                        <th scope="col"><span>제목</span></th>
                        <th scope="col"><span>작성자</span></th>
                        <th scope="col"><span>날짜</span></th>
                    </thead>

                    {/* @tbody : 글 목록 */}
                    {/* @props.articles : App.js의 contents state를 받아옴*/}
                    <tbody>{this.props.articles}</tbody>
                </table>
            </div>
        )
    }
}

export default ArticleList
```

글 목록을 표현하는 `ArticleList` 컴포넌트이다. App.js의 state 형태로 이루어진 글 목록을 props로 받아와서 테이블 형태로 뿌려준다.



```react
var navigation = (<Navi onCategoryClick={function(target) { this.setState({mode:target}); }.bind(this)}></Navi>);
    
    /* 현재 모드에 따라 컴포넌트를 생성함 */
    if(this.state.mode === 'welcome') {
      display.push(navigation);
      display.push(<Arti articles={articles}></Arti>);
      display.push(<button onClick={function() {
        this.setState({mode:'write'});
      }.bind(this)}>글쓰기</button>); // 버튼을 눌렀을 때 모드를 바꿔야하므로 컴포넌트 내부가 아닌 App.js에 생성
    } else if(this.state.mode === 'write') {
```

`App.js`에서 welcome과 관련된 구문이다. `display`는 어떤 정보를 보여줄 것인가를 나타내는 `mode`라는 state에 따라 보여주는 내용이 달라지고, navigation은 항상 그대로이다. `글쓰기`를 누르면 `mode`가 write로 바뀌면서 글쓰기로 넘어간다.



```react
      contents:[
        {id:1, title:'퇴근하고싶어용', author:'박민', date:'2020-03-16'},
        {id:2, title:'아르고스 너무 조아용 홍홍', author:'김도희', date:'2020-03-16'},
        {id:3, title:'저두용 홍홍', author:'서연주', date:'2020-03-16'},
        {id:4, title:'선배님 밥사주세용 홍홍', author:'고태완', date:'2020-03-16'},
        {id:5, title:'쓸 글이 없어용', author:'박민', date:'2020-03-16'},
        {id:6, title:'리액트 너무 재밌어용', author:'김도희', date:'2020-03-16'},
        {id:7, title:'깃허브도 너무 재밌어용', author:'서연주', date:'2020-03-16'},
        {id:8, title:'빨리 merge나 하셈용 ㅎㅎ', author:'고태완', date:'2020-03-16'},
      ]
    }
  }

  render() {
    var display = []; // 페이지를 동적으로 로드하기 위한 리스트
    var data = this.state.contents; // 글 목록
    var articles = [] // 글 목록을 props로 전달하기 위해 HTML로 묶인 것들

    /* state를 HTML 코드로 변환 => articles 리스트에 들어감 */
    for(var i = 0; i < data.length; i++) {
        console.log(data[i])
        articles.push(
            <tr>
                <td class="title">{data[i].title}</td>
                <td class="writer" width="20%">{data[i].author}</td>
                <td class="write-date" width="20%">{data[i].date}</td>
            </tr>
        )
    }
```

이건 state로 있던 글 목록을 테이블 형태로 가공하는 코드의 일부이다. article이라는 리스트에 해당 정보들이 tr과 td로 이쁘게 쌓여서 들어간다.


---


# 글쓰기 페이지

![image](https://user-images.githubusercontent.com/44183111/79989981-74347f80-84eb-11ea-97cd-44470adeb679.png)

![image](https://user-images.githubusercontent.com/44183111/79990027-844c5f00-84eb-11ea-8c64-7051a2467dac.png)

글쓰기 페이지에서는 글을 쓰면 실제로 메인 페이지에 있는 글 목록에 추가된다.



### 구현

```react
import React, { Component } from 'react';
import './Write.css';

class Write extends Component {
    /* input 내용 */
    state = {
        title: ''
    }
    
    /* input 내용이 바뀌었을 때, 이를 state에 적용함 */
    handleChange = (e) => {
        this.setState({
          title: e.target.value
        })
    }

    render() {
        return (
            <div id="WriteForm">
                <form>
                    <table id="WriteTable">
                        <tr>
                            <th scope="col">
                                Title:
                            </th>
                            <td>
                                <input
                                    placeholder='input title'
                                    value={this.state.title}
                                    onChange={this.handleChange}>
                                    </input>
                            </td>
                        </tr>
                        
                        <tr>
                            <th scope="col">
                                Content:
                            </th>
                            <td>
                                <textarea id="ContentForm" type="text" placeholder='input content'></textarea>
                            </td>
                        </tr>
                    </table>

                    
                    <button onClick={function() {
                        alert('글이 등록 되었습니다.')
                        this.props.onChangePage(this.state.title, '박민', '2020-04-02'); // App.js에 선언된 함수(state에 글 목록을 추가) 호출
                    }.bind(this)}>작성완료!</button>
                </form>
            </div>
        )
    }
}

export default Write
```

글을 다 쓰고 등록을 누르면 `props`로 전달된 글 쓰기 함수가 호출된다. 글 쓰기 함수는 `state`로 되어 있는 글 목록에 글을 추가한다.



```react
else if(this.state.mode === 'write') {
      display.push(navigation);
      display.push(<Write onChangePage={function(_title, _author, _date) {
        this.max_content_id = this.max_content_id + 1;

        /* state에 글을 추가함 */
        var _contents = this.state.contents.concat(
          {id:this.max_content_id, title:_title, author:_author, date:_date}
        );
        this.setState({contents:_contents});
        
        /* Write 컴포넌트 내부에서 제출 버튼을 눌렀을 때 모드를 변경 */
        this.setState({mode:'welcome'});
      }.bind(this)}></Write>)
```

---

# 로그인 페이지

### 디자인

![image](https://user-images.githubusercontent.com/44183111/79990480-12c0e080-84ec-11ea-8198-3a2d0bd78d81.png)

아쉽게도 진짜 로그인이 되지는 않는다..ㅎㅎ



### 구현

```react
import React, { Component } from 'react';

class LoginBox extends Component {
    render() {
        return (
            <div id="login-box">
                <table>
                    <h2><strong>로그인</strong></h2>
                    <tr>
                        <td>ID : </td> <td><input placeholder='아이디'/></td>
                    </tr>

                    <tr>
                        <td>PW : </td> <td><input placeholder='비밀번호' type='password'/></td>
                    </tr>
                    
                </table>
                <a href="">회원가입</a>
            </div>
        )
    }
}

export default LoginBoxa
```

---

# About 페이지

### 디자인

![image](https://user-images.githubusercontent.com/44183111/79991149-e5c0fd80-84ec-11ea-8513-3e27a704e688.png)



별거 없다. 그냥 텍스트만 넣었다.



### 구현

```react
import React, { Component } from 'react';

class About extends Component {
    render() {
        return (
            <div id='about-argos'>
                <h1>ARGOS</h1>
                <p>
                <strong>ARGOS</strong> 는 2003년 6월 25일에 설립된 충남대학교 컴퓨터공학과 정보보호 연구 동아리(지도교수 : 류재철 교수님) 입니다. 동아리 이름인 '아르고스'는 그리스 신화에 나오는 '100개의 눈을 가진 문지기'에서 유래되었습니다. 
                그리스 신화에 등장하는 아르고스는 잠을 잘 때도 눈을 감는 일이 없으며, 자신의 임무에 있어 적어도 하나의 눈은 대상을 응시하고 있어 창과 방패의 의미를 잘 대변할 수 있다고 할 수 있습니다.
                </p>
            </div>
        )
    }
}

export default About
```

---

# App.js

```react
/* eslint-disable import/first */

import React, { Component } from 'react';
import './App.css';

import Navi from './ire0546_Component/NavigationBar'
import Arti from './MINIBEEF_Component/ArticleList'
import Write from './HOLOLO_Component/Write'
import About from './MINIBEEF_Component/About'
import Login from './MINIBEEF_Component/LoginBox'

class App extends Component {
  constructor(props) {
    super(props);
    var max_content_id = 8;
    this.state = {

      /* mode === write : 글쓰기 모드 */
      /* mode === welcome : 글 목록 */
      mode:'welcome',

      /* 작성된 글 목록 */
      contents:[
        {id:1, title:'퇴근하고싶어용', author:'박민', date:'2020-03-16'},
        {id:2, title:'아르고스 너무 조아용 홍홍', author:'김도희', date:'2020-03-16'},
        {id:3, title:'저두용 홍홍', author:'서연주', date:'2020-03-16'},
        {id:4, title:'선배님 밥사주세용 홍홍', author:'고태완', date:'2020-03-16'},
        {id:5, title:'쓸 글이 없어용', author:'박민', date:'2020-03-16'},
        {id:6, title:'리액트 너무 재밌어용', author:'김도희', date:'2020-03-16'},
        {id:7, title:'깃허브도 너무 재밌어용', author:'서연주', date:'2020-03-16'},
        {id:8, title:'빨리 merge나 하셈용 ㅎㅎ', author:'고태완', date:'2020-03-16'},
      ]
    }
  }

  render() {
    var display = []; // 페이지를 동적으로 로드하기 위한 리스트
    var data = this.state.contents; // 글 목록
    var articles = [] // 글 목록을 props로 전달하기 위해 HTML로 묶인 것들

    /* state를 HTML 코드로 변환 => articles 리스트에 들어감 */
    for(var i = 0; i < data.length; i++) {
        console.log(data[i])
        articles.push(
            <tr>
                <td class="title">{data[i].title}</td>
                <td class="writer" width="20%">{data[i].author}</td>
                <td class="write-date" width="20%">{data[i].date}</td>
            </tr>
        )
    }

    var navigation = (<Navi onCategoryClick={function(target) { this.setState({mode:target}); }.bind(this)}></Navi>);
    
    /* 현재 모드에 따라 컴포넌트를 생성함 */
    if(this.state.mode === 'welcome') {
      display.push(navigation);
      display.push(<Arti articles={articles}></Arti>);
      display.push(<button onClick={function() {
        this.setState({mode:'write'});
      }.bind(this)}>글쓰기</button>); // 버튼을 눌렀을 때 모드를 바꿔야하므로 컴포넌트 내부가 아닌 App.js에 생성
    } else if(this.state.mode === 'write') {
      display.push(navigation);
      display.push(<Write onChangePage={function(_title, _author, _date) {
        this.max_content_id = this.max_content_id + 1;

        /* state에 글을 추가함 */
        var _contents = this.state.contents.concat(
          {id:this.max_content_id, title:_title, author:_author, date:_date}
        );
        this.setState({contents:_contents});
        
        /* Write 컴포넌트 내부에서 제출 버튼을 눌렀을 때 모드를 변경 */
        this.setState({mode:'welcome'});
      }.bind(this)}></Write>)
    } else if(this.state.mode === 'about') {
      display.push(navigation);
      display.push(<About></About>)
    } else if(this.state.mode === 'login') {
      {/* 로그인 페이지 */}
      display.push(navigation);
      display.push(<Login></Login>);
    }

    return (
      <div className="App">
        
          {display}
        
      </div>
    );
  }
}

export default App;
```

---

끝
