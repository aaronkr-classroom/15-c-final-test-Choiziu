// final_test.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char NAME_TYPE[12];

typedef struct student
{
    NAME_TYPE name;
    short lang, eng, math, sum;
    float avg;
} STUDENT;

typedef struct node
{
    STUDENT s;
    struct node* p_next;
} NODE;

void AddStudent(NODE** p_first, NODE** p_last, NODE** pp, short* num);
void ShowGrade(NODE** p_first, NODE** p_last, NODE** pp);
void CallMenu();

void LoadFromFile(NODE** p_first, NODE** p_last) {
    FILE* p_file = fopen("students.dat", "rb");
    if (p_file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    NODE* p = NULL;
    while (1) {
        NODE* new_node = (NODE*)malloc(sizeof(NODE));
        if (fread(&new_node->s, sizeof(STUDENT), 1, p_file) != 1) {
            free(new_node);
            break;
        }
        new_node->p_next = NULL;

        if (*p_first == NULL) {
            *p_first = *p_last = new_node;
        }
        else {
            (*p_last)->p_next = new_node;
            *p_last = new_node;
        }
    }

    fclose(p_file);
    printf("\n���Ͽ��� �л� �����Ͱ� �ε�Ǿ����ϴ�.\n");
}

void SaveToFile(NODE* p_first) {
    FILE* p_file = fopen("students.dat", "wb");
    if (p_file == NULL) {
        printf("\n������ �� �� �����ϴ�.\n");
        return;
    }

    NODE* p = p_first;
    while (p != NULL) {
        fwrite(&p->s, sizeof(STUDENT), 1, p_file);
        p = p->p_next;
    }

    fclose(p_file);
    printf("\n�л� �����Ͱ� ���Ͽ� ����Ǿ����ϴ�.\n");
}

int main() {
    short select, n;
    NODE* p_head = NULL, * p_last = NULL, * p = NULL;

    select = 0, n = 0;

    while (1) {
        CallMenu();
        if (scanf_s("%hu", &select) != 1) {
            rewind(stdin);
            printf("\n���ڸ� �Է��ϼ���(1 ~ 5)\n");
        }
        else if (1 == select) {
            LoadFromFile(&p_head, &p_last);  // 1. .dat ���Ͽ��� ������ �б�
        }
        else if (2 == select) {
            n++;
            AddStudent(&p_head, &p_last, &p, &n);  // 2. �߰� �л� ���� �Է�
        }
        else if (3 == select) {
            SaveToFile(p_head);  // 3. .dat ���� ����
        }
        else if (4 == select) {
            ShowGrade(&p_head, &p_last, &p);  // 4. ���� Ȯ��
        }
        else if (5 == select) {
            break;  // 5. ����
        }
        else {
            printf("\n(1 ~ 5) ��ȣ�� ���� �� �� �ֽ��ϴ�!\n");
        }
    }

    // ���� �Ҵ� �޸� ����
    p = p_head;
    while (p != NULL) {
        NODE* temp = p;
        p = p->p_next;
        free(temp);
    }

    printf("\n���α׷� ����.\n");
    return 0;
}

void CallMenu() {
    printf("\n[Menu]\n");
    printf("1. .dat ���Ͽ��� ������ �б�\n");
    printf("2. �߰� �л� ���� �Է�\n");
    printf("3. .dat ���� ����\n");
    printf("4. ���� Ȯ�� (��� ��� ��)\n");
    printf("5. ����\n");
    printf("-------------------\n");
    printf("����(1~5): ");
}

void AddStudent(NODE** p_first, NODE** p_last, NODE** pp, short* num) {
    char flag;
    flag = 0;
    *pp = *p_last;
    if (NULL != *p_first) {
        (*pp)->p_next = (NODE*)malloc(sizeof(NODE));
        *pp = (*pp)->p_next;
    }
    else {
        *p_first = (NODE*)malloc(sizeof(NODE));
        *pp = *p_first;
    }

    while (1) {
        printf("%hu��° �л� �̸� : ", *num);
        scanf_s("%s", (*pp)->s.name, sizeof((*pp)->s.name));
        printf("���� ������ �Է��ϼ��� : ");
        if (scanf_s("%hu", &(*pp)->s.lang)) {
            if (0 <= (*pp)->s.lang && 100 >= (*pp)->s.lang) {
                printf("���� ������ �Է��ϼ��� : ");
                if (scanf_s("%hu", &(*pp)->s.eng)) {
                    if (0 <= (*pp)->s.eng && 100 >= (*pp)->s.eng) {
                        printf("���� ������ �Է��ϼ��� : ");
                        if (scanf_s("%hu", &(*pp)->s.math)) {
                            if (0 <= (*pp)->s.math && 100 >= (*pp)->s.math) {
                                break;
                            }
                        }
                    }
                }
            }
        }
        printf("\n�ٽ� �Է��ϼ���.\n\n");
    }

    (*pp)->s.sum = (*pp)->s.lang + (*pp)->s.eng + (*pp)->s.math;
    (*pp)->s.avg = (float)(*pp)->s.sum / 3;
    (*pp)->p_next = NULL;
    *p_last = *pp;
}

void ShowGrade(NODE** p_first, NODE** p_last, NODE** pp) {
    char flag;
    short rank;
    STUDENT temp;

    flag = 0;
    rank = 0;
    *pp = *p_first;

    if (NULL != *pp) {
        printf("\n\n-------------------------------------\n");
        printf(" �̸�  ���� ���� ���� ���� ��� ���");
        printf("\n-------------------------------------\n");

        while (1) {
            if (NULL != (*pp)->p_next) {
                if ((*pp)->s.sum < (*pp)->p_next->s.sum) {
                    temp = (*pp)->p_next->s;
                    (*pp)->p_next->s = (*pp)->s;
                    (*pp)->s = temp;
                    flag = 1;
                }
                *pp = (*pp)->p_next;
            }
            else if (0 == flag) {
                *p_last = *pp;
                break;
            }
            else {
                flag = 0;
                *pp = *p_first;
            }
        }

        *pp = *p_first;

        while (1) {
            if (NULL != *pp) {
                printf("%6s %3hu %4hu %4hu %4hu %4.0f %3hu��\n",
                    (*pp)->s.name, 
                    (*pp)->s.lang, 
                    (*pp)->s.eng, 
                    (*pp)->s.math, 
                    (*pp)->s.sum, 
                    (*pp)->s.avg, ++rank);
                *pp = (*pp)->p_next;
            }
            else {
                break;
            }
        }
    }
    else {
        printf("\n��ϵ� �л��� �����ϴ�.\n");
    }
}