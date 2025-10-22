# 2024-MCU-STM32

# STM32 기반 실시간 차량 기능 시뮬레이터
> (Real-Time Vehicle Function Simulator using STM32)

## 1. Overview

마이크로프로세서 설계 및 디자인 수업의 팀 프로젝트로, `**STM32**` 보드를 사용하여 차량의 핵심 전자 시스템을 모델링하고 시뮬레이터로 구현했습니다.

이 프로젝트를 통해 `**C언어**`와 `**Assembly**`를 이용한 로우레벨 하드웨어 제어 및 실시간 시스템의 기본 원리를 학습했습니다. 이는 향후 V2X(차량 통신) 및 자율주행 시스템 연구에 필요한 차량 임베디드 시스템의 기초가 되었습니다.

## 2. Implemented Features

* **DC motor control:** 설정된 속도 값을 초과할 경우 LED와 LCD를 통해 실시간으로 운전자에게 경고합니다.
* **Switch Interrupt:** 외부 스위치 입력을 받아 좌/우측 방향 지시등 LED를 제어합니다.
* **Streamlit Interface(UART 통신):** 현재 속도 및 차량 상태를 컴퓨터 모니에 실시간으로 출력합니다.

## 3. Core Concepts & Tech Stack

* **Platform:** STM32F4xx
* **Languages:** `**C**`, `**Assembly**`
* **Key Concepts:**
    * `**DC motor control**`: 가변저항 값을 조절하여 전진 상태를 구현하고, 과속 제한을 위해 일정 속도 이상이 되면 경고음과 text LCD를 출력하고, 속도 limit을 걸었습니다.
    * `**Switch Interrupts**`: 스위치 입력(방향 지시등)을 즉각적으로 처리하기 위해 인터럽트 방식을 사용했습니다.
    * `**Streamlit Interface(UART 통신)**`: Streamlit Interface를 사용하여 속도 계기판을 구현하였습니다.

## 4. 소프트웨어 구성 및 flow chart

### Initial Setting

<img width="900" height="444" alt="image" src="https://github.com/user-attachments/assets/ef5687b5-b831-4bb6-932a-b43250e4bc8f" />

### Interrupt

<img width="872" height="366" alt="image" src="https://github.com/user-attachments/assets/0943b11d-10d8-4e7f-b0a5-b63a73ef06e4" />

### Main Loop

<img width="785" height="772" alt="image" src="https://github.com/user-attachments/assets/2f124ec8-2e5f-4774-80b3-4701831ec8db" />

## 5. 통합 시뮬레이션 및 구현

![0-5](https://github.com/user-attachments/assets/038f1a52-24f3-472a-9cdf-d58f4222f750)
![5-10](https://github.com/user-attachments/assets/417bbf19-5d4c-4b2a-84c7-aa0027d83d82)
![11-16](https://github.com/user-attachments/assets/0d4c2e18-5ba7-4442-a88b-43c512aadeaa)
![17-22](https://github.com/user-attachments/assets/66b022b3-c3f2-4086-bb9b-da9a0fe3de87)
![23-28](https://github.com/user-attachments/assets/cc1352b6-a098-4675-91e5-6a2ec9afa377)
![29-34](https://github.com/user-attachments/assets/c0dbc75d-df83-476a-a423-bc374f093d60)
![35-40](https://github.com/user-attachments/assets/5278076c-2a24-49be-ae1a-0448f966c877)
![41-46](https://github.com/user-attachments/assets/3c3b6205-e38a-4b70-bc38-6fd9d577596d)
![47-52](https://github.com/user-attachments/assets/d33c8ec4-6a71-4b82-9c55-198529816ec5)
![53-58](https://github.com/user-attachments/assets/db55ecee-32e7-49c8-8e07-b992d8fa28ac)
![58-61](https://github.com/user-attachments/assets/da70ecf7-14e1-47a6-8cdb-430d2afc520a)
![62-64](https://github.com/user-attachments/assets/769ca55c-9991-4281-899b-abae2736b68d)
