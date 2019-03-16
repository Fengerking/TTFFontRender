#include <stdio.h>
#include <assert.h>
#include <string>
#include <time.h>

const int integer_range = 100;

typedef struct
{
	int numerator;					//����
	int denominator;				//��ĸ
	bool is_positive;				//�Ƿ�Ϊ����
}fraction_s;

typedef enum {
	operation_add,				//������-��
	operation_sub,					//������-��
	operation_mul,				//������-��
	operation_div,					//������-��
	operation_left_bracket,	//������-������
	operation_right_bracket	//������-������
}operation_e;

char operation_str[6] = { '+' , '-' , '*' , '/' , '(' , ')'};

//���ʽ
typedef struct {
	int operand_num;				//����������		
	fraction_s * operands;			//����������
	operation_e * operations;	//���������У�����Ϊ����������-1)
	int bracket_num;					//���Ŷ���
	int * left_bracket_index;		//���е��������±�
	int * right_bracket_index;	//���е��������±�
}expression_s;

//-----�����������-----������-----//
//��ShowFraction�����Խ��
//���飺��������ȫ���ֿ� �����ʵ�ֲ���ֻ�����ֵ 
//����Ҫ�ڼ���֮ǰ����ת�� 
//���� -1 �� 3 תΪ �� 3 �� 1 
//-1 - 3 תΪ�� 1 + 3 Ȼ��ı� is_positive ����
//�ڼ��������ֵ֮���ٿ��ǽ���������� �������ܻ��������һЩ
//��������ܴ���ø������Ҳ���Բ���������

//����ֵ�������� ������f1-������ f2������
fraction_s FractionAdd(const fraction_s & f1 , const fraction_s & f2)
{

}

//����ֵ�������� ������f1-������ f2������
fraction_s FractionSub(const fraction_s & f1, const fraction_s & f2)
{

}

//����ֵ�������� ������f1-�˺������� f2-�˺��Ҳ����
fraction_s FractionMul(const fraction_s & f1, const fraction_s & f2)
{

}

//����ֵ�������� ������f1-������ f2-���� (ע��0�Ĵ���)
fraction_s FractionDiv(const fraction_s & f1, const fraction_s & f2)
{

}

//------------------------------------------------------------------------------------

bool FractionEqual(const fraction_s & f1, const fraction_s & f2)
{
	return (f1.numerator == 0 && f2.numerator == 0) || 
		(f1.numerator == f2.numerator && f1.denominator == f2.denominator);
}

//������Լ��

int GetGCD(int a, int b)
{
	if (a == 0 || b == 0 ) return 1;
	if (a < b)
	{
		int c = a;
		a = b;
		b = c;
	}
	int remain;
	do {
		remain = a % b;
		a = b;
		b = remain;
	} while (remain != 0);
	return a;
}

void FractionReduction(fraction_s & f)
{
	int gcd = GetGCD(f.numerator, f.denominator);
	f.numerator /= gcd;
	f.denominator /= gcd;
}

//�������
void ShowFraction(const fraction_s & f)
{
	if (!f.is_positive) printf("-");
	printf("%d", f.numerator);
	if (f.denominator == 1)  return;
	printf("/%d", f.denominator);
}

//����һ���������ʽ

expression_s CreateExpression(int operand_num , bool need_bracket , bool is_only_integer)
{
	expression_s new_expression;
	new_expression.operand_num = operand_num;
	new_expression.operands = (fraction_s*)malloc(sizeof(fraction_s) * operand_num);
	new_expression.operations = (operation_e*)malloc(sizeof(operation_e) * (operand_num - 1));

	//������ɲ�����
	for (int i = 0; i < operand_num; i++)
	{
		new_expression.operands[i].numerator = rand() % integer_range;
		if (is_only_integer)
		{
			new_expression.operands[i].denominator = 1;
		}
		else {
			int r;
			do {
				r = rand() % integer_range;
			} while (r == 0); //ȷ����ĸ��Ϊ0
			new_expression.operands[i].denominator = r;
		}
		FractionReduction(new_expression.operands[i]);
	}

	//���ɲ�����
	for (int i = 0; i < operand_num - 1; i++)
	{
		int r = rand() % 4;
		if (new_expression.operands[i].numerator == 0) r = rand() % 3; //�������0
		new_expression.operations[i] = (operation_e)r;
	}

	//��������
	if (need_bracket)
	{
		new_expression.bracket_num = operand_num >> 2;
		new_expression.left_bracket_index = (int*)malloc(sizeof(int) * new_expression.bracket_num);
		new_expression.right_bracket_index = (int*)malloc(sizeof(int) * new_expression.bracket_num);

		for (int i = 0; i < new_expression.bracket_num; i++)
		{
			int l_b, r_b;
			bool has_same_bracket;
			do {
				has_same_bracket = false;
				l_b = rand() % (new_expression.operand_num - 2);
				int r_b_range = new_expression.operand_num - l_b - 1;
				if (r_b_range == 0)
				{
					r_b = new_expression.operand_num - 2;
				}
				else {
					r_b = l_b + 1 + rand() % r_b_range;
				}
				if (l_b == r_b)
				{
					continue;
				}
				for (int j = 0; j < i; j++)
				{
					if (new_expression.left_bracket_index[j] == l_b &&
						new_expression.right_bracket_index[j] == r_b)
					{
						has_same_bracket = true;
					}
				}
			} while (has_same_bracket );

			new_expression.left_bracket_index[i] = l_b;
			new_expression.right_bracket_index[i] = r_b;

		}
	}

	return new_expression;
}

//����������ʽ
void ShowExpression(const expression_s & s)
{
	for (int i = 0; i < s.operand_num; i++)
	{
		//������ܴ��ڵ�������
		for (int j = 0; j < s.bracket_num; j++)
		{
			if (s.left_bracket_index[j] == i) printf("(");			
		}

		//���������
		ShowFraction(s.operands[i]);

		//������ܴ��ڵ�������
		for (int j = 0; j < s.bracket_num; j++)
		{
			if (s.right_bracket_index[j] == i) printf(")");
		}

		//���������
		if (i < s.operand_num - 1)
		{
			if (s.operations[i] == operation_div)
			{
				char * s = "��";
				printf("%c%c", s[0] , s[1]);
			}
			else printf("%c", operation_str[s.operations[i]]);
		}

	}

	printf("\n");
}

//------- ���ʽ��ֵ -------  ������//

//����ֵ��������ý��
//��ShowFraction�����Խ��
//���ա����ݽṹ�����ʽ��ֵ�ķ��� �ж�ĳ��������֮ǰ��֮���Ƿ������� ��Ҫ�������е����ŵ��±� ���Բο� ShowExpression()
//�����������ȼ���δ���� ������)
fraction_s GetExpressionVal(const expression_s & s)
{

}


//------------------------------------------------------------------------------------

int main()
{
	srand(time(0));
	/*
	��������ʾһ�����ʽ�Ĺ��� ����������в���
	expression_s s = CreateExpression(10 , true , false);
	ShowExpression(s);
	*/
	return 0;
}