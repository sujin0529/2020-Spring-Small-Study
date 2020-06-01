---
title: "[Misc] Principal Components Analysis"
date: 2020-04-20
categories: Misc
sitemap :
  changefreq : daily
  priority : 1.0
---



# PCA(Principal Components Analysis)



## 1) Dimension Reduction

![image](https://user-images.githubusercontent.com/44183111/79761851-f5114100-835c-11ea-9e70-df1c901c443e.png)

 우리가 실제 세계(Real World)를 분석할 때 자연의 존재들을 Sensing해서 Data로 가공하게 된다. 이때 데이터는 수학이라는 도구를 적용시키기 쉽도록 Vector Form으로 변환하는 것이 일반적이다(선형 대수적).



![image](https://user-images.githubusercontent.com/44183111/79761938-0fe3b580-835d-11ea-983b-e9e3f327faa5.png)

 아닌 경우도 있지만 자연의 구성 요소를 Vector Form으로 가져왔을 때 Dimension이 매우 크기 때문에 수학적 연산이 곤란하다. 이 때문에 우리는 Simantic은 유지하면서 Dimension을 축소시킬 방법이 필요하다(단순화). Dimension을 줄이는 것은 데이터를 표현하는데 용량을 줄일 수 있고, 이는 Machine Learning 함에 있어 처리 속도를 기하 급수적으로 줄일 수 있다. 또한 이를 2~3차원으로 축소시킬 시 다양한 그래프로 시각화가 가능하다. 그런 측면에서 Dimension Reduction은 매우 중요하다. 이 때 많은 Component중 어떤 것이 중요하고 어떤 것이 필요 없는지 아는 것이 핵심이다(골격이 되는 데이터는 무엇인가?). 



![image](https://user-images.githubusercontent.com/44183111/79762010-212cc200-835d-11ea-8b8e-d8c2f81bdbe2.png)

 이를 대수적으로 따져보면 위의 그림과 같다. Dimenstion Reduction을 통해 K 차원을 M 차원으로 변환하는 것이다.



![image](https://user-images.githubusercontent.com/44183111/79762051-2f7ade00-835d-11ea-8d20-9783205f03bf.png)

 Dimenstion Reduction에는 두가지 방식이 있다. 첫 번째는 Feature Selection인데, 용어 그대로 26개의 차원(A to Z)이 있다고 했을 때 우리에게 필요한 Component를 기계적으로 Selection해서 가져오겠다는 것이다. 특징은 Selection된 데이터가 그대로 사용된다는 것이다(Selection된 Component의 변형이 전혀 없다). 두 번째는 Feature Projection 인데, 어떠한 수학적 방법이던 취해서 다른 공간으로 Projection하는 것을 말한다. Feature Extraction이라고도 불리며 일반적으로 Dimenstion Reduction 이라 하면 Projection을 말한다. 이번 주제인 PCA(Principal Component Analysis) 에서 가장 중요한 테크닉이다.



## 2) **PCA(Pricipal Component Analysis)**

![image](https://user-images.githubusercontent.com/44183111/79762122-4a4d5280-835d-11ea-8ea7-f84f27d1d3f4.png)

 우리가 시험 과학자라고 가정해보자. 우리가 센서를 통해 관측한 것들은 대부분 정확한 결과가 아니다. 일단 센싱한 다음 얻어진 데이터를 통해서 현상을 해석할 뿐이다. 즉 현상을 정확히 이해하고 이를 분석하는 것이 아닌, 실제 세계(Real World)에 다양한 센서를 덕지덕지 붙여 관찰하고, 얻어낸 데이터를 이용해 현상의 분석을 정립하는 것이다. 이런 상황일 때 데이터를 어떻게 의미를 갖도록 할 것인가가 PCA의 의도이다.



![image](https://user-images.githubusercontent.com/44183111/79762292-8b456700-835d-11ea-8709-feec484b83bd.png)

 우리는 위 그림과 같은 실험에서 빨간 공을 +x 방향으로 잡아당겼다가 놓았을 때 용수철의 탄성에 의해서 오직 x축으로만 운동할 것을 이론적으로 알고 있다. 하지만, 만약에 우리가 이러한 사전 이론 없이 오직 ‘카메라’라는 센서만 이용해서 현상을 관찰하고 분석한다고 가정하자. 아마 카메라가 관측하는 빨간 공의 운동은 정확한 xyz 축에 의한 것으로 해석될 수 없을 것이다. 단지 실험에 의한 데이터만 가지고 추측할 뿐이다.



![image](https://user-images.githubusercontent.com/44183111/79762338-98faec80-835d-11ea-9370-ed0f206b96a4.png)

 카메라 A, B, C는 정확하게 90도를 이루지는 않고 있다. 이 때 120Hz로 측정한 데이터이다. 각 카메라가 얻어낸 세 개의 데이터는 각기 다르지만, 이들을 비교 분석하여 빨간 공의 운동을 알아낼 수 있다. 핵심은, 이러한 복잡하고 난해한 상황에서 어떻게 “빨간공은 x축으로만 움직인다.”는 간단한 결론에 도달할 수 있는가이다. 우리는 이를 PCA를 통해 할 수 있다.



![image](https://user-images.githubusercontent.com/44183111/79762456-c5166d80-835d-11ea-8b45-c3365b3b60e3.png)

 카메라 A의 경우를 생각해보자. A의 경우 120Hz로 사진을 찍었을 때 공의 움직임이 x와 y 이차원의 사진 형태로 나올 것이다. 물론 이는 B, C 카메라에서도 마찬가지이다. 우리는 이 데이터를 각 카메라가 찍은 사진마다 X, Y로 각각 나누어 총 6개(2 * 3) 차원의 Component를 얻어낼 수 있다. 이를 10분간 촬영했다 하면 72000개의 샘플이 나오게 될 것이다. 그렇게 해서 6개 컴포넌트 * 72000개 샘플로 이루어진 데이터 셋을 확인할 수 있다.



### 2-1) Linear Data Projection

![image](https://user-images.githubusercontent.com/44183111/79762456-c5166d80-835d-11ea-8b45-c3365b3b60e3.png)

이를 PCA를 통해 축소하는 첫 번째 방식은 Linear하게 Projection하는 것이다.



![image](https://user-images.githubusercontent.com/44183111/79762611-ef682b00-835d-11ea-9951-b09ffbeb170d.png)
![image](https://user-images.githubusercontent.com/44183111/79762618-f131ee80-835d-11ea-81c9-8d0cad4dd475.png)

 그 방법은 바로 Linear Transformation이다. 선형 변환(Linear Transformation)이란 어떤 데이터를 Rotation 시키고 Stretch 시키는 것을 말한다. Stretch를 시켰을 때 방향이 바뀌지 않는 Vector들이 있는데, 이들을 고유 벡터(Eigen Vector)라고 부른다.



### 2-2) Basis Change

![image](https://user-images.githubusercontent.com/44183111/79762732-16bef800-835e-11ea-92a7-9fe5a6e4ee5f.png)

 다시 72000개의 데이터로 돌아와서 X를 우리가 가진 데이터 셋이라고 하면, 이를 Basis로 표현했을 때 위 그림에서 아래 행렬과 같다(IX =X). 우리는 직교성을 띄는 X의 축을 다른 차원으로 바꿀 것이다.



![image](https://user-images.githubusercontent.com/44183111/79762843-37874d80-835e-11ea-9108-546bc0c9132f.png)

 데이터셋 X의 축을(Basis)를 변환하여(Projection) 해당 현상을 더 잘 설명할 수 있는 새로운 데이터 셋인 Y로 만들 것이다. P는 새로운 데이터 셋의 Basis가 된다. 그렇다면 X의 내용을 해치지 않으면서 P를 어떠한 방식으로 설정할 것인가? -> Minimize Redundancy!



### 2-3) Minimize Redundancy

 아이디어는 “Data를 표현하는 각 차원 간의 정보 중복이 없도록 데이터 재구성” 하는 것이다. 아래 그림을 보자.

![image](https://user-images.githubusercontent.com/44183111/79762913-54238580-835e-11ea-8132-8433c80b05d1.png)

![image](https://user-images.githubusercontent.com/44183111/79762927-5980d000-835e-11ea-93d5-b2559301a324.png)

 만약 A가 움직일 때 D도 따라서 움직인다면 A와 D 둘 모두의 정보가 필요 없다는 것이다. 즉 A만 있으면 D도 모델링 할 수 있다는 것이다. 그 반대라면 A와 D 모두의 정보가 필요하다는 것이다. 이는 정보 축약의 여지의 여부를 판가름한다(Redundancy).



![image](https://user-images.githubusercontent.com/44183111/79763046-7fa67000-835e-11ea-8d39-e2c5b02e7413.png)

두 컴포넌트간 유사도는 covariance matrix를 이용한다. Redanduncy를 측정하는 방식중 하나로 covariance matrix를 쓰는 것이다.



![image](https://user-images.githubusercontent.com/44183111/79763085-8af99b80-835e-11ea-9352-0993738fa712.png)

![image](https://user-images.githubusercontent.com/44183111/79763104-92b94000-835e-11ea-8b2b-e14479cc5e8f.png)

 PCA를 통해 우리가 구하고자 하는 것은 P인데, Redundant를 마쳤을 때 결과인 Y 데이터 셋은 서로 Not-Redundant 해야 한다. 즉, 모든 컴포넌트들이 연관이 없어야 한다는 것이다. 즉 diagonal을 제외한 모든 컴포넌트에서 covariance가 0이 나와야 한다.



![image](https://user-images.githubusercontent.com/44183111/79763129-9d73d500-835e-11ea-950a-5fc84502f4f4.png)

covariance matrix는 다음과 같이 구할 수 있다.