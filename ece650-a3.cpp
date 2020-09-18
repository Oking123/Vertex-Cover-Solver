#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

int temp(){
    while(!std::cin.eof()){
    std::string line;
    std::getline(std::cin, line);
    std::cout<<line<<std::endl;
    }
    return 0;
}


int main (int argc, char **argv) {
    std::vector<pid_t> kids;
    int rgentoa1[2];
    int a1toa2[2];
    pipe(rgentoa1);
    pipe(a1toa2);

    pid_t child_pid;
    child_pid = fork();
    if (child_pid == 0) {
        // deliver the output of rgen to a1 by pipe(rgentoa1)
        dup2(rgentoa1[1], STDOUT_FILENO);
        close(rgentoa1[1]);
        close(rgentoa1[0]);
        close(a1toa2[0]);
        close(a1toa2[1]);
        execv("./rgen", argv);
        return 0;
    }
    kids.push_back(child_pid);

    pid_t child_pid2;
    child_pid2 = fork();
    if (child_pid2 == 0) {
        dup2(rgentoa1[0], STDIN_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        dup2(a1toa2[1], STDOUT_FILENO);
        close(a1toa2[1]);
        close(a1toa2[0]);
        char *arg1[2];
        arg1[0] = (char *) "ece650-a1.py";
        arg1[1] = nullptr;
        execv("./ece650-a1.py", arg1);
        return 0;
    }
    kids.push_back(child_pid2);
    pid_t child_pid3;
    child_pid3 = fork();
    if (child_pid3 == 0) {
        dup2(a1toa2[0], STDIN_FILENO);
        close(a1toa2[1]);
        close(a1toa2[0]);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        char *arg2[2];
        arg2[0] = (char *) "ece650-a2";
        arg2[1] = nullptr;
        execv("./ece650-a2", arg2);

        dup2(a1toa2[1], STDOUT_FILENO);
        close(rgentoa1[0]);
        close(rgentoa1[1]);
        close(a1toa2[1]);
        close(a1toa2[0]);
        temp();
        return 0;
    }
    kids.push_back(child_pid3);

    pid_t child_pid4;
    child_pid4 = fork();
    if (child_pid4 == 0) {
    dup2(a1toa2[1], STDOUT_FILENO);
    close(rgentoa1[0]);
    close(rgentoa1[1]);
    close(a1toa2[1]);
    close(a1toa2[0]);
    temp();
    kill(child_pid3, SIGTERM);
    kill(child_pid2, SIGTERM);
    kill(child_pid, SIGTERM);
//    for (pid_t k : kids) {
//            kill(k, SIGTERM);
//    }
    return 0;
}
    kids.push_back(child_pid4);
    int status;
    waitpid(child_pid4, &status, 0);
//    int status;
//    waitpid(child_pid, &status, 0);
//    for (pid_t k : kids) {
//        kill(k, SIGTERM);
//    }
    return 0;


}
