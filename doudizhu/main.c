#include<stdio.h>
#include"head.h"
int main(){
	while (1){
		int which;
		game_init();//��ʼ����Ϸ
		while (1){
			turning();//�غϽ�����
			printf("\n\n\n");
			if (which = win())//�Ƿ����ʤ��
				break;
			turn_switch();//�л����Ʒ�
		}
		printf("ʤ���ѳ���ʤ���������%d\n", which + 1);
		printf("�Ƿ�������Ϸ?(yΪ�����������˳�����):");
		char choice;
		scanf("%c", &choice);
		scanf("%*[^\n]");
		scanf("%*c");
		if (choice == 'y' || choice == 'Y')
			continue;
		break;
	}
	printf("лл����\n");
	return 0;
}