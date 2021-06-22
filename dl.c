#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
void ls(int argc, char *argv[]); //디렉토리를 열고 파일상태출력 함수를 제어함
void printStat(char *type_t,char *pathname, char *file, struct stat *st); //파일 상태를 출력하는 함수
char type(mode_t mode); //파일의 타입을 출력함
char *perm(mode_t mode); //파일의 접근 권한을 출력함
void help(); //프로그램의 사용방법에 대해 출력함 
void version(); //프로그램의 버전명을 출력함
int main(int argc, char *argv[]){
	if(strcmp(argv[1], "-help") == 0){
		help();
		exit(1);
	}
	else if(strcmp(argv[1], "-version") == 0){
		version();
		exit(1);
	}
	else
		ls(argc, argv);	//입력된 어규먼트의 갯수와 어규먼트를 ls 함수에 입력
}
void ls(int argc, char *argv[]){
	DIR *dp; //조사할 디렉토리
	char *dir; //문자형 디렉토리 주소
	struct dirent *d; //파일 또는 디렉토리가 가지고 있는 정보 구조체
	struct stat st; //파일의 정보를 받아옴
	char path[BUFSIZ+1]; //디렉토리 경로
	if(argc == 2 || argc == 1) //만약 어규먼트의 개수가 1개이거나 2개이면 
		dir = "."; // 현재 디렉토리내에서 명령을 실행하게 된다.
	else dir = argv[2]; //그게 아니라면 실행할 디렉토리는 2번째 어규먼트가 된다.

	if((dp = opendir(dir)) == NULL)//만약 열어야될 디렉토리가 비어있거나 없나면
	 perror(dir); //dir에 대해 오류 출력
	while ((d = readdir(dp)) != NULL) { //파일 또는 디렉토리를 열었을때 값이 null이 아닐때 까지
	 sprintf(path, "%s/%s", dir, d->d_name); //path에다가 경로를 출력 
	if(lstat(path, &st) < 0) //만약 지정된 파일의 상태를 반환하는데 실패하면
	 perror(path); // path에 대해 오류 출력
	else{ //오류가 출력되지 않았다면
	 if(argc == 1) //아무런 명령어 옵션이 없을때
		printStat("n", path, d->d_name, &st); //파일 이름만 출력하는 옵션 실행

	 else //그게 아니라면
	 	printStat(argv[1], path, d->d_name, &st); //주어진 옵션에 맞는 명령 실행
		}
	}
	closedir(dp); //디렉토리 닫음
	exit(0); //시스템 종료
}
void help(){ 
	printf("-l : 디렉토리내에 모든 파일들의 요소들을 전부 다 보여줍니다.\n");
	printf("-s : 디렉토리내에 모든 파일 혹은 지정된 파일의 사이즈를 출력합니다.\n");
	printf("-c : 디렉토리내에 모든 파일 혹은 지정된 파일의 마지막 수정 시간을 보여줍니다.\n");
	printf("-u : 디렉토리내에 모든 파일 혹은 지정된 파일의 마지막 접근 시간을 보여줍니다.\n");
	printf("-n : 디렉토리내에 모든 파일 혹은 지정된 파일의 소유자의 이름을 보여줍니다.\n");
}
void version(){
	printf("Version : 1.0.0\n");
}

void printStat(char *type_t, char *pathname, char *file, struct stat *st){
	if(strcmp(type_t, "-l") == 0){ //주어진 명령어 옵션이 -l이라면 주어진 디렉토리 내에 모든 파일 정보를 출력
		printf("%5ld ", st -> st_blocks);
		printf("%c%s ", type(st->st_mode), perm(st->st_mode));
		printf("%3ld ", st->st_nlink);
		printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
		printf("%9ld ", st->st_size);
		printf("%.12s ", ctime(&st->st_mtime)+4);
		printf("%s\n", file);
	}
	else if(strcmp(type_t, "-s") == 0){ //주어진 명령어 옵션이 -s라면 주어진 디렉토리 내에 모든 파일의 용량과 파일명을 출력
		printf("%9ld ", st->st_size);
		printf("%s\n", file);
	}
	else if(strcmp(type_t, "-c") == 0){ //주어진 명령어 옵션이 -c라면 주어진 디렉토리 내에 모든 파일의 마지막 파일 상태 변경 시간과 파일명 출력
		printf("%.12s ", ctime(&st->st_mtime)+4);
		printf("%s\n", file);
	}
	else if(strcmp(type_t, "-a") == 0){ //주어진 명령어 옵션이 -a라면 주어진 디렉토리 내에 모든 파일명을 출력
		printf("%s\n", file);
	}
	else if(strcmp(type_t, "-u") == 0){ //주어진 명령어 옵션이 -u라면 주어진 디렉토리 내에 모든 파일의 마지막 수정 시간과 파일명 출력
		printf("%.12s ", ctime(&st->st_atime)+4);
		printf("%s\n", file);
	}
	else if(strcmp(type_t, "-n") == 0){ //주어진 명렁어 옵션이 -n이라면 주어진 디렉토리 내에 모든 파일의 소유자 이름과 권한등과 파일명 출력
		printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
		printf("%s\n", file);
		}
	//else if(strcmp(type_t, "") == 0){
	//}
	else if(strcmp(type_t, "n") == 0) printf("%s\n", file);	//옵션 없는 상태라면 파일명만 출력한다.
}

char type(mode_t mode){
	if(S_ISREG(mode)) //해당 파일이 일반 파일인지 체크한다.
	 return('-'); //맞다면 '-'를 리턴 
	if(S_ISDIR(mode)) //해당 파일이 디렉토리 파일인지 체크한다.
	 return('d'); //맞다면 'd'를 리턴
	if(S_ISCHR(mode)) //해당 파일이 문자장치 파일인지 체크한다. 
	 return('c'); //맞다면 'c'를 리턴
	if(S_ISBLK(mode)) //해당 파일이 블록 장치 파일인지 체크한다.
	 return('b'); //맞다면 'b'를 리턴
	if(S_ISLNK(mode)) //해당 파일이 심볼릭 링크 파일인지 체크한다.
	 return('l'); //맞다면 'l'을 리턴
	if(S_ISFIFO(mode)) //해당 파일이 FIFO인지 체크한다.
	 return('p'); //맞다면 'p'를 리턴
	if(S_ISSOCK(mode)) //해당 파일이 소켓 파일인지 체크한다.
	 return('s'); //맞다면 's'를 리턴
}
char *perm(mode_t mode)
{
	int i;
	static char perms[10] = "----------"; //사용 권한을 받고 출력하기 위한 배열 생성
	
	for (i=0; i<3; i++) {
	 if(mode & (S_IRUSR >> i*3)) //소유자의 읽기 권한이 있는지 체크
		perms[i*3] = 'r'; // 있다면 비트 연산에서 나온 값 만큼 r을 배열에 대입
	 if(mode & (S_IWUSR >> i*3)) //소유자의 쓰기 권한이 있는지 체크
		perms[i*3+1] = 'w'; // 있다면 비트 연산에서 나온 값 만큼 w를 배열에 대입
	 if(mode & (S_IXUSR >> i*3)) // 소유자의 쓰기 권한이 있는지 체크
		perms[i*3+2] = 'x'; // 있다면 비트 연산에서 나온 값 만큼 x를 배열에 대입 
	}
	
	return(perms); //배열 perms을 리턴 
}

