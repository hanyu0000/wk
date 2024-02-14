#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<limits.h>
/*
DIR结构体：DIR结构体用于表示目录流。它是一个不透明的结构，通常由目录操作函数返回。在程序中，你不需要直接访问它的成员，只需将它作为参数传递给相关的目录操作函数即可。
  d_ino：    inode 号码，是文件系统中唯一标识文件或目录的值。
  d_off：    目录项在目录文件中的偏移量。
  d_reclen： 目录项长度。
  d_type：   文件类型。
  d_name：   文件或目录的名称，是一个以 null 结尾的字符串。
struct dirent结构体：struct dirent结构体用于存储目录中的一个边界的信息。它包含了文件或子目录的名称等信息。
opendir函数：opendir函数用于打开一个目录，并返回一个指向DIR结构体的指针。
readdir函数：readdir函数用于读取目录流中的下一个边界，并返回一个指向struct dirent结构体的指针。
closedir函数：closedir用于关闭目录流的函数，释放相关的资源。
*/
int lab_a = 0;
int lab_l = 0;
int lab_R = 0;
int lab_t = 0;
int lab_r = 0;
int lab_i = 0;
int lab_s = 0;
void myls();
int compare_mtime(const void *a, const void *b);
void ls_l();
void ls_R(); // 递归列表
void ls_r();
void ls_i();
void ls_s();
int main(int argc, char *argv[]) {
    // 解析命令行参数并设置标志
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0)
            lab_a = 1;
        else if (strcmp(argv[i], "-l") == 0)
            lab_l = 1;
        else if (strcmp(argv[i], "-R") == 0)
            lab_R = 1;
        else if (strcmp(argv[i], "-t") == 0)
            lab_t = 1;
        else if (strcmp(argv[i], "-r") == 0)
            lab_r = 1;
        else if (strcmp(argv[i], "-i") == 0)
            lab_i = 1;
        else if (strcmp(argv[i], "-s") == 0)
            lab_s = 1;
    }
    // 如果没有提供目录参数，默认使用当前目录
    if (argc == 1)
        myls(".");
    else {
        // 对于每个提供的目录参数，调用 myls 函数
        for (int i = 1; i < argc; i++)
            myls(argv[i]);
    }
    return 0;
}
void myls(char *name) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(name);
    if (dir == NULL) {
        perror("打开文件错误");
        exit(1);
    }
    // 遍历目录内容
    while ((entry = readdir(dir)) != NULL) {
        // 如果启用了 -a 选项或者该文件不是隐藏文件，则打印条目
        if (lab_a || entry->d_name[0] != '.')
            printf("\033[32m%-8s\033[0m", entry->d_name);
        // 根据启用的选项调用不同的函数
        if (lab_l)
            ls_l(name, entry->d_name);
        if (lab_R)
            ls_R(name); // 递归列表
        if (lab_r)
            ls_r(name);
        if (lab_i)
            ls_i(name, entry->d_name);
        if (lab_s)
            ls_s(name, entry->d_name);
    }
    // 如果启用了 -t 选项，则按时间排序并打印文件列表
    if (lab_t) {
        // 获取文件列表
        struct dirent **namelist;
        int n = scandir(name, &namelist, NULL, alphasort);
        if (n < 0) {
            perror("扫描目录错误");
            exit(EXIT_FAILURE);
        }
        // 打印按时间排序的文件列表
        ls_t(name, namelist, n);
        // 释放内存
        for (int i = 0; i < n; i++)
            free(namelist[i]);
        free(namelist);
    }
    closedir(dir);
}
// 比较函数，用于排序
int compare_mtime(const void *a, const void *b) {
    struct stat *stat_a = (struct stat *)a;
    struct stat *stat_b = (struct stat *)b;
    return stat_b->st_mtime - stat_a->st_mtime;
}
void ls_l(char *name, char *entry_name) {
    struct stat info;
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", name, entry_name);
    if (stat(full_path, &info) != -1) {
        printf("%s ", S_ISDIR(info.st_mode) ? "d" : "-"); // File type
        printf((info.st_mode & S_IRUSR) ? "r" : "-");
        printf((info.st_mode & S_IWUSR) ? "w" : "-");
        printf((info.st_mode & S_IXUSR) ? "x" : "-");
        printf(" ");
        printf((info.st_mode & S_IRGRP) ? "r" : "-");
        printf((info.st_mode & S_IWGRP) ? "w" : "-");
        printf((info.st_mode & S_IXGRP) ? "x" : "-");
        printf(" ");
        printf((info.st_mode & S_IROTH) ? "r" : "-");
        printf((info.st_mode & S_IWOTH) ? "w" : "-");
        printf((info.st_mode & S_IXOTH) ? "x" : "-");
        printf(" ");
        printf("%ld ", info.st_nlink); // Number of hard links
        struct passwd *pw = getpwuid(info.st_uid);
        printf("%s ", pw->pw_name); // Owner
        struct group *gr = getgrgid(info.st_gid);
        printf("%s ", gr->gr_name); // Group
        printf("%lld ", (long long)info.st_size); // File size
        struct tm *tm_info;
        char buffer[26];
        tm_info = localtime(&info.st_mtime);
        strftime(buffer, 26, "%Y-%m-%d %H:%M ", tm_info); // Last modification time
        printf("%s ", buffer);
        printf("%s\n", entry_name);
    }
}
void ls_t(char *name, struct dirent **namelist, int n) {
    struct stat *stat_list;
    stat_list = (struct stat *)malloc(n * sizeof(struct stat));
    if (stat_list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    // 获取每个文件的信息
    for (int i = 0; i < n; i++) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", name, namelist[i]->d_name);
        if (stat(full_path, &stat_list[i]) != -1) {
            // 跳过隐藏文件
            if (!lab_a && namelist[i]->d_name[0] == '.')
                continue;
        }
    }
    // 按照修改时间排序
    if (lab_t)
        qsort(stat_list, n, sizeof(struct stat), compare_mtime);
    // 输出文件列表
    for (int i = 0; i < n; i++) {
        if (lab_l) {
            ls_l(name, namelist[i]->d_name);
        } else {
            // 输出文件名
            printf("%s\n", namelist[i]->d_name);
        }
    }
    // 释放内存
    free(stat_list);
}
void ls_R(char *name) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(name);
    if (dir == NULL) {
        perror("Error open file");
        exit(1);
    }
    printf("%s:\n", name); // 打印目录名
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (lab_a || entry->d_name[0] != '.') {
                printf("\033[32m%-8s\033[0m", entry->d_name);
                if (lab_l) {
                    ls_l(name, entry->d_name); // 输出详细信息
                }
                printf("\n");
            }
            if (entry->d_type == DT_DIR) {
                char sub_path[1024];
                snprintf(sub_path, sizeof(sub_path), "%s/%s", name, entry->d_name);
                ls_R(sub_path); // 递归调用
            }
        }
    }
    closedir(dir);
}
void ls_r(char *name) {
    DIR *dir;
    struct dirent **namelist;
    int n;
    dir = opendir(name);
    if (dir == NULL) {
        perror("Error open file");
        exit(1);
    }
    n = scandir(name, &namelist, NULL, lab_r ? alphasort : alphasort); // 根据lab_r选择排序方式
    if (n < 0) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    while (n--) {
        struct dirent *entry = namelist[n];
        if (lab_a || entry->d_name[0] != '.') {
            printf("\033[32m%-8s\033[0m", entry->d_name);
            if (lab_l)
                ls_l(name, entry->d_name);
            printf("\n");
        }
    }
    if (lab_R) {
        ls_R(name); // 如果启用了递归选项，则调用递归函数
    }
    // 释放内存
    for (int i = 0; i < n; i++)
        free(namelist[i]);
    free(namelist);
    closedir(dir);
}
void ls_i(char *name, char *entry_name) {
    struct stat info;
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", name, entry_name);
    if (stat(full_path, &info) != -1) {
        printf("%llu ", (unsigned long long)info.st_ino); 
    }
}
void ls_s(char *name, char *entry_name) {
    struct stat info;
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", name, entry_name);
    if (stat(full_path, &info) != -1) {
        printf("%ld ", (long)info.st_blocks); 
    }
}