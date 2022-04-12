# Pintos Project
2020-Fall Sogang Univ. 운영체제(CSE4070) 과목 프로젝트

## Project1. User Program(1)
User program이 Pintos 위에서 정상적으로 구동되도록 하기 위한 기능 구현
* Argument Passing
  * 입력으로 들어온 command line에서 argument들을 parsing하고 80x86 calling convention에 맞춰 user stack에 쌓이도록 함
* User Memory Access
  * user program이 kernel 영역을 침범하게 될 경우 invalid pointer로 판단하여 작동하지 못하도록 하는 기능 구현
*	System Calls
    * Pintos manual에 있는 system calls 중 halt, exit, exec, wait 기능을 완성
    * read, write에 대해서는 standard input/output이 가능할 정도로 구현
    * 추가적인 system call인 fibonacci, max_of_four_int를 구현


## Project2. User Program(2)
프로젝트 1에서 구현하지 않은 file system 관련 system call 기능을 완성
* File Descriptor
  * thread마다 개별적인 file descriptor table을 생성
  * 파일을 열 때마다 서로 다른 file descriptor 값을 부여하여 file descriptor table을 통한 파일 관리가 가능하도록 함
* System Calls
  * Pintos manual에 있는 system calls 중 create, remove, open, close, filesize, read, write, seek, tell 기능을 완성
    * open 과정에서 executable file에 대한 쓰기 작업이 수행되지 않도록 보호
  * 프로젝트 1에서 구현했던 read STDIN, write STDOUT과 더불어 File system과 관련된 모든 system call 기능이 제대로 수행되도록 함
* Synchronization in Filesystem
  * file system에서의 synchronization 기능을 구현 
    * 코드의 critical section을 보호하고 각 프로세스가 shared data에 독점적으로 접근하도록 함


## Project3. Threads
* Alarm clock
  * 기존 Pintos에서 Busy waiting으로 구현된 Alarm 기능 보완
    * 정해진 시간이 되지 않은 스레드를 Block 시키는 방법으로 Sleep과 Wake up 기능을 구현, Alarm 기능이 보다 효율적으로 실행될 수 있도록 함 
* Priority Scheduling
  * 기존 Pintos의 Round-robin scheduler 보완
    * 새로운 스레드가 삽입될 때 priority를 고려하도록 하여 스레드가 각자의 priority에 맞는 순서대로 ready queue에 삽입되고 수행될 수 있도록 함
* Advanced Scheduler
  * BSD Scheduler 구현
    * recent_cpu와 nice의 값을 이용하여 스레드의 priority를 계산하므로 기존 방식보다 효율적인 threads scheduling 가능
