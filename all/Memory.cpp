#include<iostream>
#include<stdlib.h>
#define Free 0 //����״̬
#define Busy 1 //����״̬
#define OK 1    //���
#define ERROR 0 //����
#define MAX_length 640 //����ڴ�ռ�Ϊ640 KB

typedef int Status;
using namespace std;
int n=0;

typedef struct freearea//����һ��������˵����ṹ
{
	int ID;   //������
	long size;   //������С
	long address; //������ַ
	int state;   //״̬
}ElemType;

//---------- ���Ա��˫������洢�ṹ ------------
typedef struct DuLNode //double linked list
{
	ElemType data;
	struct DuLNode *prior; //ǰ��ָ��
	struct DuLNode *next; //���ָ��
}DuLNode,*DuLinkList;

DuLinkList block_first; //ͷ���
DuLinkList block_last; //β���

Status alloc(int);//�ڴ����
Status free(int); //�ڴ����
Status First_fit(int,int);//�״���Ӧ�㷨
Status Best_fit(int,int); //�����Ӧ�㷨
void show();//�鿴����
Status Initblock();//�����ռ��

Status Initblock()//������ͷ�����ڴ�ռ�����
{
	block_first=(DuLinkList)malloc(sizeof(DuLNode));
	block_last=(DuLinkList)malloc(sizeof(DuLNode));
	block_first->prior=NULL;
	block_first->next=block_last;
	block_first->data.state=Busy;
	block_first->data.ID=-1;
	block_last->prior=block_first;
	block_last->next=NULL;
	block_last->data.address=0;
	block_last->data.size=MAX_length;
	block_last->data.ID=0;
	block_last->data.state=Free;
	return OK;
}

//----------------------- �� �� �� �� -------------------------
Status alloc(int ch)
{
	int ID,request;
	cout<<"��������ҵ(������)��";
	cin>>ID;
	cout<<"��������Ҫ����������С(��λ:KB)��";
	cin>>request;
	if(request<0 ||request==0)
	{
		cout<<"�����С�����ʣ�������!"<<endl;
		return ERROR;
	}
	
	if(ch==2) //ѡ�������Ӧ�㷨
	{
		if(Best_fit(ID,request)==OK) cout<<"����ɹ�!"<<endl;
		else cout<<"�ڴ治�㣬����ʧ��!"<<endl;
		return OK;
	}
	else //Ĭ���״���Ӧ�㷨
	{
		if(First_fit(ID,request)==OK) cout<<"����ɹ�!"<<endl;
		else cout<<"�ڴ治�㣬����ʧ��!"<<endl;
		return OK;
	}
}

//------------------ �״���Ӧ�㷨 -----------------------
Status First_fit(int ID,int request)//������ҵ����������
{
	//Ϊ������ҵ�����¿ռ��ҳ�ʼ��
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
	temp->data.ID=ID;
	temp->data.size=request;
	temp->data.state=Busy;
	
	DuLNode *p=block_first->next;
	while(p)
	{
		if(p->data.state==Free && p->data.size==request)
		{//�д�Сǡ�ú��ʵĿ��п�
			p->data.state=Busy;
			p->data.ID=ID;
			return OK;
			break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//�п��п���������������ʣ��"
			temp->prior=p->prior;
			temp->next=p;     
			temp->data.address=p->data.address;
			p->prior->next=temp;
			p->prior=temp;
			p->data.address=temp->data.address+temp->data.size;
			p->data.size-=request;
			return OK;
			
			break;
		}
		p=p->next;
	}
	return ERROR;
}

//-------------------- �����Ӧ�㷨 ------------------------
Status Best_fit(int ID,int request)
{
	int ch; //��¼��Сʣ��ռ�
	DuLinkList temp=(DuLinkList)malloc(sizeof(DuLNode));
	temp->data.ID=ID;
	temp->data.size=request;
	temp->data.state=Busy;
	DuLNode *p=block_first->next;
	DuLNode *q=NULL; //��¼��Ѳ���λ��
	while(p) //��ʼ����С�ռ�����λ��
	{
		if(p->data.state==Free &&
	    	(p->data.size>request || p->data.size==request) )
		{
			q=p;
		    ch=p->data.size-request;
		    break;
		}
		p=p->next;
	}
	
	while(p)
	{
		if(p->data.state==Free && p->data.size==request)
		{//���п��Сǡ�ú���
		    p->data.ID=ID;
		    p->data.state=Busy;
		    return OK;
		    break;
		}
		if(p->data.state==Free && p->data.size>request)
		{//���п���ڷ�������
			if(p->data.size-request<ch)//ʣ��ռ�ȳ�ֵ��С
		    {
		    	ch=p->data.size-request;//����ʣ����Сֵ
		    	q=p;//�������λ��ָ��
		    }
		}
		p=p->next;
	}
	if(q==NULL) return ERROR;//û���ҵ����п�
	else
	{//�ҵ������λ�ò�ʵ�ַ���
		temp->prior=q->prior;
		temp->next=q;
		temp->data.address=q->data.address;
		q->prior->next=temp;
		q->prior=temp;
		q->data.address+=request;
		q->data.size=ch;
		return OK;
	}
}

//-----------------------   �� �� �� ��   --------------------
Status free(int ID)
{
	DuLNode *p=block_first;
	while(p)
	{
		if(p->data.ID==ID)
		{
			p->data.state=Free;
		    p->data.ID=Free;
		    if(p->prior->data.state==Free&&p->next->data.state==Free)//ǰ���������п��п�
		    {
				p->prior->data.size+=p->data.size;
				p->prior->data.size+=p->next->data.size;
			    if(p->next->next)
			    p->next->next->prior=p->prior;
			    p->prior->next=p->next->next;
			    break;
		    }
		    if(p->prior->data.state==Free)//��ǰ��Ŀ��п�����
		    {
			    p->prior->data.size+=p->data.size;
			    p->prior->next=p->next;
			    p->next->prior=p->prior;
		    }
		    if(p->next->data.state==Free)//�����Ŀ��п�����
		    {
			    p->data.size+=p->next->data.size;
			    if(p->next->next)
			     	p->next->next->prior=p;
			    p->next=p->next->next;    
		    }
		    break;
	   }
	   p=p->next;
	}
	return OK;
}

//--------------- ��ʾ���������� ------------------
void show()
{
	cout<<endl;
	cout<<" �� �� �� �� �� �� :\n";
	DuLNode *p=block_first->next;
	while(p)
	{
		cout<<"�� �� �ţ�";
	    if(p->data.ID==Free) cout<<"Free"<<endl;
	    else cout<<p->data.ID<<endl;
	    cout<<"��ʼ��ַ��"<<p->data.address<<endl;
	    cout<<"������С��"<<p->data.size<<" KB"<<endl;
	    cout<<"״    ̬��";
	    if(p->data.state==Free) cout<<"�� ��"<<endl;
	    else cout<<"�ѷ���!"<<endl;
	    cout<<"-----------------------"<<endl;
	    p=p->next;
	}
}

//----------------------- �� �� ��---------------------------
int main()
{
	int ch,d=0;//�㷨ѡ����
	cout<<"  ��ѡ���㷨�� " <<endl;
	cout<<"1.�״���Ӧ�㷨 2.�����Ӧ�㷨 0.�˳� " <<endl;
	
	cout<<"��ѡ������㷨��";
	cin>>ch;
	if(ch==0||ch==1||ch==2) d++;
	while(d==0)
	{
		cout<<"��ѡ��0 ,1 ��2"<<endl;
		cin>>ch;
		if(ch==0||ch==1||ch==2) d++;
	}
	if(ch==0) exit(0);
	//if(n==0) Initblock(); //�����ռ��ʽ1
	Initblock();//�����ռ��ʽ2
	            //���ߵ��������ڣ���ʽ1����ת����һ���㷨ʱ�����ֵ�һ���㷨���ڴ�����������ʽ2
	            //��ʽ2����ת����һ���㷨ʱ�����·������е��ڴ棬��ԭΪ��ʼ״̬�����ַ�ʽ������ѡ��
	int choice; //����ѡ����
	while(1)
	{
	    cout<<endl;
	    cout<<"1: �����ڴ�        2: �����ڴ�\n";
	    cout<<"3: �鿴����        0: ��    ��\n";
	    cout<<"���������Ĳ��� ��";
	    cin>>choice;
		if(choice==1)
		{
			alloc(ch); // �����ڴ�
		    n++;
		}
		else if(choice==2) // �ڴ����
		{
		    int ID;
		    cout<<"��������Ҫ�ͷŵķ����ţ�";
		    cin>>ID;
		    free(ID);
		    n++;
		}
		else if(choice==3)
		{
		    show();//��ʾ����
		    n++;
		}
		else if(choice==0)
		{
		    main(); //�˳�
		    n++;
		}
		else //�����������
		{
		    cout<<"��������������!"<<endl;
		    continue;
		}
	}
	return 0;
}

