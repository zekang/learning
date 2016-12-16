#include<stdio.h>
#include<stdlib.h>
#include"head.h"
#include<string.h>
#include <time.h>
static int  type_buffer();
static char point[] = { '0', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K', 'A', '2', 'w', 'W' };
static char *farr[] = { "��Ƭ", "÷��", "����", "����" };
static char* type_arr[] = { "��Ȩ", "����", "����", "��ը", "����", "���õ�", "ը��", "���ö�", "ը����", "˳��", "ը��һ��", "�ɻ�����", "����", "�ɻ���", "�ɻ�����" };
static char* sta_arr[2] = { "ũ��", "����" };
static players player[3];//���
static games game;
static char lastPlayerSendCard[32] = { 0 };





static void saveLastPlayerSendCard(int *brr, card *handcard)
{
	int i;
	for (i = 0; brr[i]; i++){
		lastPlayerSendCard[i] = handcard[brr[i]].point;
	}
	lastPlayerSendCard[i] = 0;
}

static void printLastPlayerSendCard()
{
	for (int i = 0; lastPlayerSendCard[i]; i++){
		if (lastPlayerSendCard[i] == 'X'){
			printf("%s ", 10);
		}
		else if (lastPlayerSendCard[i] == 'w'){
			printf("%s ", "С��");
		}
		else if (lastPlayerSendCard[i] == 'W'){
			printf("%s ", "����");
		}
		else{
			printf("%c ", lastPlayerSendCard[i]);
		}
	}
	printf("\n");
}

/*ϴ�ƶ�*/
static deck* deck_init(){
	int i, j;
	srand((unsigned int ) time(NULL));
	deck *p_deck = (deck*)malloc(sizeof(deck));
	if (!p_deck){
		printf("�����ڴ�ʧ��\n");
		return NULL;
	}
	for (i = 1; i <= 54; i++){
		p_deck->arr[i] = rand() % 54;
		for (j = 1; j<i; j++){
			if (p_deck->arr[i] == p_deck->arr[j]){
				i--;
				break;
			}
		}
	}
	p_deck->top = 54;
	return p_deck;
}

/*��ʼ�����(ϴ�ƣ�id����� ����,����)*/
static void player_init(){
	int i, j;
	for (j = 0; j<3; j++){
		for (i = 1; i <= 20; i++){
			player[j].handcard[i].number = 100;
			player[j].handcard[i].level = 0;
		}
	}
	deck *p = deck_init();
	if (!p){
		printf("û���ƶ�\n");
		return;
	}
	int which = 0;
	which = rand() % 3;
	game.who = which;
	for (i = 0; i<3; i++){
		player[i].id = i;
		if (i == which){//����
			player[i].status = 1;
			for (j = 1; j <= 20; j++){
				player[i].handcard[j].number = p->arr[(p->top)--];
			}
			player[i].size = 20;
		}
		else{//ũ��
			player[i].status = 0;
			for (j = 1; j <= 17; j++){
				player[i].handcard[j].number = p->arr[(p->top)--];
			}
			player[i].size = 17;
		}
	}
	free(p);
	p = NULL;
}

/*������Ϣ����*/
static void handcard_init(){
	int i, j;
	for (i = 0; i<3; i++){
		for (j = 1; j <= 20; j++){
			int number = player[i].handcard[j].number;
			int *p_level = &(player[i].handcard[j].level);
			char **pp_flower = &(player[i].handcard[j].flower);
			char *p_point = &(player[i].handcard[j].point);
			if (number >= 0 && number <= 51){
				*p_level = number / 4 + 1;
				*p_point = point[number / 4 + 1];
				*pp_flower = farr[number % 4];
			}
			else if (number == 52){
				*p_level = 14;
				*p_point = 'w';
				*pp_flower = "С��";
			}
			else if (number == 53){
				*p_level = 15;
				*p_point = 'W';
				*pp_flower = "����";
			}
			else {
				*p_level = 0;
				*p_point = ' ';
				*pp_flower = " ";
			}
		}
	}
}

/*��ӡ��ǰ�������*/
void print(){
	int i, j;
	for (i = 0; i<3; i++){
		if (i != game.who) continue;
		for (j = 1; j <= player[i].size; j++){
			//printf("======");
			if (player[i].handcard[j].number == 100){
				printf("  ");
			}
			else {
				char *p_tmp = player[i].handcard[j].flower;
				printf("%s ", p_tmp);
				
			}
		}
		printf("\n");
		for (j = 1; j <= player[i].size; j++){
			if (player[i].handcard[j].number == 100){
				printf(" ");
			}
			else {
				printf("  %c  ", player[i].handcard[j].point);
				if (j==10){
					printf(" ");
				}
			}
		}
	}
	printf("\n");
	for (j = 1; j <= player[game.who].size; j++){
		if (!(j>9))
			printf(" (%d) ", j);
		else
			printf(" (%d)", j);
	}
	printf("\n");
}

/*��Ϸ��ʼ��*/
void game_init(){
	game.count = 2;//��Ȩ����
	player_init();//ϴ�� ����
	handcard_init();//������Ϣ����
}

int fcmp(const void *pa, const void *pb){//����
	return *(int*)pa - *(int*)pb;
}

static void rehandcard_sort(players *p_player, int* p_number){//������������
	int *base = p_number;
	size_t nmemb = p_player->size;
	size_t size = sizeof(card);
	qsort(base, 20, size, fcmp);
}

void handcard_sort(){//���������
	rehandcard_sort(&player[0], &(player->handcard[1].number));
	rehandcard_sort(&player[1], &((player + 1)->handcard[1].number));
	rehandcard_sort(&player[2], &((player + 2)->handcard[1].number));
}

/*ѯ���Ƿ����*/
static int require(){ //1��ʾ���� 0��ʾ��Ȩ
	if (game.type == 3){
		if (game.count != 2){
			printf("Ҫ����!\n");
			return 0;
		}
		else
			return 1;
	}
	if (game.count != 2){
		printf("������?(y��ʾ���ƣ�n��Ȩ):");
		char choice;
		scanf("%c", &choice);
		scanf("%*[^\n]");
		scanf("%*c");
		if (choice == 'n' || choice == 'N'){
			return 0;
		}
		else return 1;
	}
	else {
		printf("��������\n");
		return 1;
	}
}

buffers buffer = {0};//���ƻ�����

/*��ճ��ƻ�����*/
static void reset(){
	int a;
	for (a = 0; a<16; a++)
		buffer.arr[a] = 0;
	for (a = 0; a<20; a++)
		buffer.brr[a] = 0;
	buffer.sum = 0;
}

/*���ƽ��뻺����*/
static void buffer_put(){
	char intput[40] = {0};//���ַ���ת��Ϊ����
	int brr[20] = {0};
	int i = 0;
	int j;
	int sum;
	int flag = 0;
	int len;
	while (1){
		reset();
	sig:  printf("������Ҫ�ŵ���...:");
		fgets(intput, 40, stdin);
		if (strlen(intput) == 39 && intput[38] != '\n'){
			scanf("%*[^\n]");
			scanf("%*c");
		}
		for (j = 0, i = 0, sum = 0, len = strlen(intput); i<len; i++){//��¼�����±�
			if (intput[i] >= '0'&&intput[i] <= '9'){
				sum = sum * 10 + (intput[i] - '0');
				flag = 1;
			}
			else {
				if (flag)
					brr[j] = sum;
				sum = 0;
				j++;
				flag = 0;
			}
		}
		int k;
		printf("Ҫ��: ");
		for (k = 0; brr[k]; k++)
			printf("%d ", brr[k]);
		printf("����\n");
		int who = game.who;
		players* p_player = &(player[who]);
		int index;
		for (i = 0; brr[i]; i++){//��¼��������ʲô
			index = brr[i];
			if (index>(p_player->size) || index <= 0){//������ַ�����Χ����
				printf("�������ݳ�����Χ����������\n");
				goto sig;
			}
			else{
				int level = p_player->handcard[index].level;
				++(buffer.arr[level]);
				buffer.brr[i] = brr[i];
			}
		}
		for (i = 1; i <= 15; i++)//��¼���˶�������
			buffer.sum += buffer.arr[i];
		char aff = 'N';
		int type = type_buffer();
		if (type != -1)
			printf("Ҫ����������:%s\n\n", type_arr[type]);
		else {
			printf("�����ڴ����͵���\n\n");
			reset();
			return;
		}
		printf("ȷ��Ҫ��������?(ȷ������y,��������)");
		scanf("%c", &aff);
		scanf("%*[^\n]");
		scanf("%*c");
		if (aff == 'y' || aff == 'Y')
			break;
	}
}

static void turnstart(){
	char u;
	printf("\n\n==========================������========================\n\n");
	printf("�ֵ���һ��");
	scanf("%c", &u);
	printf("\n\n��������Ϊ%d��\n", game.sum);
	printf("��������Ϊ:  %s%c\n", type_arr[game.type], point[game.level]);
	printLastPlayerSendCard();
	printf("=================================%s�Ļغ�========================\n", sta_arr[player[game.who].status]);
	printf("�����ֵ����%d\n", game.who + 1);
	printf("\t���1(%s)����%d ", sta_arr[player[0].status], player[0].size); if (game.who == 0) printf("<=====\n"); else printf("\n");
	printf("\t���2(%s)����%d ", sta_arr[player[1].status], player[1].size); if (game.who == 1) printf("<=====\n"); else printf("\n");
	printf("\t���3(%s)����%d ", sta_arr[player[2].status], player[2].size); if (game.who == 2) printf("<=====\n"); else printf("\n");
}

/*�ж��Ƿ�����*/
static int continuum(int num, int total){
	int i, count = 0;
	int flag = 0;//��ֵ����Ϊ1
	int sig = 0;//�� �е��� �ı��Ϊ1
	for (i = 1; i <= 15; i++){
		if (buffer.arr[i] == num){
			if (sig)
				return 0;//������
			count++;
			if (count == total)
				return 1;//����
			flag = 1;
		}
		else {
			if (flag)
				sig = 1;
		}
	}
	return 0;
}

/*��ȡ��������������*/
static int type_buffer(){
	int i, one = 0, pair = 0, triple = 0, quattuor = 0, zero = 0;
	for (i = 1; i <= 15; i++){//ͳ�Ƶ��ţ����ӣ���ͬ����ͬ���ж���
		if (buffer.arr[i] == 1)
			one++;
		else if (buffer.arr[i] == 2)
			pair++;
		else if (buffer.arr[i] == 3)
			triple++;
		else if (buffer.arr[i] == 4)
			quattuor++;
		else zero++;
	}
	//printf("��%d ��%d ��%d ��%d ��%d,sum%d===\n",one,pair,triple,quattuor,zero,buffer.sum);
	/* 
	static char* type_arr[] = { 
	"��Ȩ", 
	"����", "����", "��ը", "����", "���õ�", 
	"ը��", "���ö�", "ը����", "˳��", "ը��һ��",
	"�ɻ�����", "����", "�ɻ���", "�ɻ�����" 
	};
	*/
	if (!(buffer.sum)){
		return -1;//�Ƿ�
	}
	else if (buffer.sum <= 5){//1~5
		if (one == 1 && !pair && !triple && !quattuor)//����
			return 1;
		else if (pair == 1 && !one && !triple && !quattuor)//����
			return 2;
		else if (one == 2 && buffer.arr[14] && buffer.arr[15])//��ը
			return 3;
		else if (triple == 1 && !one && !pair && !quattuor) //����
			return 4;
		else if (one == 1 && !pair && triple == 1 && !quattuor)//���õ�
			return 5;
		else if (!one && !pair && !triple && quattuor == 1)//ը
			return 6;
		else if (!one && pair == 1 && triple == 1 && !quattuor)//���ö�   
			return 7;
		else if (one == 1 && !pair && !triple && !quattuor){//ը���� 
			return 8;
		}
		else if (!pair && !triple && !quattuor && (!buffer.arr[14] && !buffer.arr[15]) && buffer.sum == 5){//˳��
			if (continuum(1, one))//����1����
				return 9;
			else {
				return -1;
			}
		}
		else return -1;
	}
	else if (buffer.sum >= 6){
		if ((!one) && (pair == 1) && (!triple) && (quattuor == 1))//ը����
			return 10;
		else if (!one && !pair && !quattuor){//ֻ��2�����ϸ�������ͬ �ɻ�����
			if (continuum(3, triple))//����3����
				return 11;
			else return -1;
		}
		else if (!one && !triple && !quattuor){//����
			if (continuum(2, pair))
				return 12;
			else return -1;
		}
		else if (buffer.sum == 4 * triple){//�ɻ���
			if (continuum(3, triple))
				return 13;
			else return -1;
		}
		else if ((buffer.sum == 5 * triple) && (triple == pair)){//�ɻ���
			if (continuum(3, triple))
				return 14;
			else return -1;
		}
		else if (!pair && !triple && !quattuor && (!buffer.arr[14] && !buffer.arr[15])){
			if (continuum(1, one))
				return 9;
			else return -1;
		}
		else return -1;
	}
	return -1;
}

/*����±�*/
static int maxindex(int count){
	int i;
	for (i = 15; i >= 1; i--){
		if (buffer.arr[i] == count)
			return i;
	}
	return -1;
}

/*��ȡ����������ȼ�*/
static int level_buffer(int type){
	switch (type){
	case 1:
		return maxindex(1);
		break;
	case 2:
		return maxindex(2);
		break;
	case 3:
		return 15;
		break;
	case 4:
		return maxindex(3);
		break;
	case 5:
		return maxindex(3);
		break;
	case 6:
		return maxindex(4);
		break;
	case 7:
		return maxindex(3);
		break;
	case 8:
		return maxindex(4);
		break;
	case 9:
		return maxindex(1);
		break;
	case 10:
		return maxindex(4);
		break;
	case 11:
		return maxindex(3);

		break;
	case 12:
		return maxindex(2);
		break;
	case 13:
		return maxindex(3);
		break;
	case 14:
		return maxindex(3);
		break;
	}
	return -1;
}

/*��������*/
static void annealation(){
	int i = 1;
	int j = 0;
	int who = game.who;
	saveLastPlayerSendCard(buffer.brr, player[who].handcard);
	for (i = 1, j = 0; buffer.brr[j]; i++, j++){
		int index = buffer.brr[j];
		player[who].handcard[index].number = 100;
		player[who].size = player[who].size - 1;
	}
	game.sum = buffer.sum;
	game.count = 0;
	handcard_sort();
	//printf("���Ƴɹ�\n");
}

/*�غϽ�����*/ //���ģ���кܴ���޸Ŀռ� ����return ��Ϊbreak...
void turning(){
	turnstart();              /* ��ʼ�׶� */
	handcard_sort();
	print();
	int level = 0;
	while (1){
		if (!require()){
			printf("\n��Ҫ\n");
			game.count++;
			if (game.count == 2){
				game.type = 0;
				game.level = 0;
				game.sum = 0;
			}
			return;            /* ��ʼ�׶� */
		}
		buffer_put();            /* ���ƽ׶����⺯���ж��Ƿ���Ȩ�ȽϺ� */
		int type = type_buffer();
		int level = level_buffer(type);
		if (type == -1){
			printf("����Ƿ�!-----\n");
			continue;
		}
		if (type == 3){//��ը
			printf("\n\n\n\n\n��ը!!\n");
			annealation();
			game.type = 3;
			game.level = MAXLEVEL;
			return;
		}
		else if (type == 6){//ը��
			if (game.type != 6){
				printf("\n\n\n\nը��\n");
				annealation();
				game.type = 6;
				game.level = level_buffer(type);
				return;
			}
			else {
				if (level > game.level){
					printf("\n\n\n\n\nѹ��\n");
					annealation();//��������
					game.type = 6;
					game.level = level;
					return;
				}
				else if (level < game.level || level == game.level){
					printf("�Ƶĵȼ�������\n");
					continue;
				}
			}
		}
		else if (game.count == 2){//������Ȩ
			annealation();
			game.type = type;
			game.level = level;
			return;
		}
		else {//����ը�� ������Ȩ ��ը �Ƿ� ����ĺ�������
			if (type != game.type){ //����Ӧ
				printf("���Ͳ���Ӧ\n");
				continue;
			}
			else { //��Ӧ
				if (buffer.sum != game.sum){
					printf("��������Ӧ\n");
					continue;
				}
				if (level < game.level || level == game.level){
					printf("�Ƶĵȼ�������\n");
					continue;
				}
				else if (level > game.level){
					printf("\n\n\n\n\nѹ��\n");
					annealation();
					game.type = type;
					game.level = level;
					return;
				}
			}
		}
	}
}

/*0 1 2 3�ж��Ƿ���˭ʤ�� 0��ʾû�� 1��ʾ���һ*/
int win(){
	int now = game.who;
	if (!player[now].size)
		return now;
	else return 0;
}

/*�л���ǰ���Ϊ�¼�*/
void turn_switch(){
	int who = game.who;
	who++;
	game.who = who % 3;
}