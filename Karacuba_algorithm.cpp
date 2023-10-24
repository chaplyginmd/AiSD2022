#include <iostream>
#include <algorithm>
#include <ranges>
#include <functional>
#include <cmath>
#include <string>
#include <cassert>
#include <random> 

using namespace std;

/*
	ASCII
	0 -> 48
	1 -> 49
	...
	9 -> 57
*/

//сменить знак числа, представленного в виде строки
void change_sign(string& s)
{
	string zero(s.size(), '0');
	if (s[0] == '-') s.erase(0, 1);
	else if(s != zero) s.insert(0, "-");
}

//заполнить нулями до определенной длины
void expand_with0(string& num, size_t const& size)
{
	char beg = num[0];
	if (beg == '-')
	{
		change_sign(num);
		expand_with0(num, size);
		change_sign(num);
		return;
	}
	size_t len = num.size();
	do
	{
		num.insert(0, "0");
		++len;
	} while (len < size);
}

//убрать лишние нули
string erase0(string const& s)
{
	string res = s;
	char ch = res[0];
	size_t len = res.size();

	if (ch == '-')
	{
		change_sign(res);
		res = erase0(res);
		change_sign(res);
	}

	while (ch == '0' && len > 1)
	{
		res.erase(0, 1);
		ch = res[0];
		--len;
	}
	return res;
}

//возвращает модуль числа, представленного строкой
string modul(string const& s)
{
	char beg = s[0];
	string res = s;

	if (beg == '-') return res.erase(0, 1);
	else return res;
}

//вычислить длину целого числа, представленного строкой
size_t len(string const& s)
{
	char beg = s[0];
	if (beg == '-') return s.size() - 1;
	return  s.size();
}

//сложить два неотрицательных числа, представленных в виде строк
string plus_as_num(string const& num1, string const& num2)
{
	size_t
		len1 = num1.size(),
		len2 = num2.size(),
		len = len1 < len2 ? len2 : len1,
		flag = 0,
		dig = 0,
		sum_last = 0;
	string
		n1 = num1,
		n2 = num2,
		res;

	if (len1 < len2) expand_with0(n1, len);
	else if (len1 > len2) expand_with0(n2, len);

	for (size_t i = 0; i < len; ++i)
	{
		sum_last = n1[len - 1 - i] + n2[len - 1 - i] + flag - 96;
		dig = sum_last % 10;
		res.insert(0, to_string(dig));
		flag = sum_last / 10;
	}

	if (flag != 0) res.insert(0, to_string(flag));

	return res;

}

//Разность двух неотрицательных чисел, представленных в виде строк, причем вычитаемое не превосходит уменьшаемое
string dif_as_num(string const& num1, string const& num2)
{
	size_t
		len1 = num1.size(),//"опорная" длина
		len2 = num2.size();
	int dif_last = 0;
	string
		n1 = num1,
		n2 = num2,
		res;

	if (len1 > len2) expand_with0(n2, len1);

	for (size_t i = 0, j = 1; i < len1; ++i)
	{
		char& reduced = n1[len1 - 1 - i], deductible = n2[len1 - 1 - i];
		dif_last = reduced - deductible;

		while (dif_last < 0 && len1 - 1 - i - j >= 0)
		{
			char& next_reduced = n1[len1 - 1 - i - j];
			if (next_reduced != '0')
			{
				dif_last += 10;
				--next_reduced;
				j = 1;
				break;
			}
			next_reduced = '9';
			++j;
		}

		if ((i < len1 - 1) || (dif_last != 0 && i == len1 - 1) || (i == len1 -1 && dif_last == 0)) res.insert(0, to_string(dif_last));
	}

	return res;
}

//разность любых чисел, представленных строкой
string dif_all(string const& num1, string const& num2)
{
	string
		n1 = num1,
		n2 = num2,
		res;
	size_t
		len1 = len(n1),
		len2 = len(n2);

	if (len1 < len2) expand_with0(n1, len2);
	else if (len1 > len2) expand_with0(n2, len1);

	string
		absn1 = modul(n1),
		absn2 = modul(n2);

	char beg1 = n1[0], beg2 = n2[0];

	if (beg1 != '-' && beg2 != '-')
	{
		if (n1 >= n2) return erase0(dif_as_num(n1, n2));
		else return erase0('-' + dif_as_num(n2, n1));
	}

	else if (beg1 == '-' && beg2 == '-')
	{
		if (absn1 >= absn2) return erase0('-' + dif_as_num(absn1, absn2));
		else return erase0(dif_as_num(absn2, absn1));
	}

	else if (beg1 != '-' && beg2 == '-') return erase0(plus_as_num(n1, absn2));
	else if (beg1 == '-' && beg2 != '-') return erase0('-' + plus_as_num(absn1, n2));
}

//сложение любых целых чисел, представленных строкой
string plus_all(string const& num1, string const& num2)
{
	string
		n1 = num1,
		n2 = num2;

	char beg1 = n1[0], beg2 = n2[0];

	if (beg1 != '-' && beg2 != '-') return erase0(plus_as_num(n1, n2));
	else if (beg1 == '-' && beg2 == '-')
	{
		change_sign(n1);
		change_sign(n2);
		return erase0('-' + plus_as_num(n1, n2));
	}
	else if (beg1 != '-' && beg2 == '-')
	{
		change_sign(n2);
		return dif_all(n1, n2);
	}
	else if (beg1 == '-' && beg2 != '-') return erase0(plus_all(n2, n1));
}

//умножение двух положительных целых n разрядных десятичных чисел, представленных строкой
string Karacuba_alg(string const& num1, string const& num2)
{
	string
		n1 = num1,
		n2 = num2;
	size_t len1 = n1.size(), len2 = n2.size();

	if (len1 < len2) expand_with0(n1, len2);//n1.insert(0, "0");
	else if (len1 > len2) expand_with0(n2, len1);//n2.insert(0, "0");

	size_t len = max(len1, len2);
	

	if (len1 == 1 && len2 == 1) return to_string((n1[0] - 48) * (n2[0] - 48));
	else
	{
		size_t const
			first_half_size = len - len / 2,
			pow10 = pow(10, len - first_half_size);

		auto const p1 = n1.begin();
		auto const p2 = n2.begin();
		auto const q1 = n1.end();
		auto const q2 = n2.end();

		string
			a1(p1, p1 + first_half_size),
			b1(p1 + first_half_size, q1),
			a2(p2, p2 + first_half_size),
			b2(p2 + first_half_size, q2);

		string
			//a1 * a2
			a1a2 = Karacuba_alg(a1, a2),
			//b1 * b2
			b1b2 = Karacuba_alg(b1, b2),
			//a1 + b1
			a1pb1 = plus_as_num(a1, b1),
			//a2 + b2
			a2pb2 = plus_as_num(a2, b2),
			//(a1 + b1)(a2 + b2)
			a1pb1a2pb2 = Karacuba_alg(a1pb1, a2pb2);

		//a1a2 * pow10 * pow10 + ((a1 + b1)(a2 + b2) - a1a2 - b1b2) * pow10 + b1b2

		a1pb1a2pb2 = dif_all(dif_all(a1pb1a2pb2, a1a2), b1b2);
		a1pb1a2pb2 = a1pb1a2pb2 == "0" ? "0" : a1pb1a2pb2.append(len - first_half_size, '0');
		a1a2 = a1a2 == "0" ? "0" : a1a2.append(2 * (len - first_half_size), '0');

		return plus_all(a1a2, plus_all(a1pb1a2pb2, b1b2));
	}
}

//умножение двух целых n разрядных десятичных чисел, представленных строкой
string Karacuba_algorithm(string const& num1, string const& num2)
{
	string
		absn1 = modul(num1),
		absn2 = modul(num2);

	char beg1 = num1[0], beg2 = num2[0];

	if ((beg1 != '-' && beg2 != '-') || (beg1 == '-' && beg2 == '-')) return Karacuba_alg(absn1, absn2);
	else if ((beg1 != '-' && beg2 == '-') || (beg1 == '-' && beg2 != '-')) return erase0('-' + Karacuba_alg(absn1, absn2));
}

int main()
{
	mt19937_64 rng(random_device{}());
	uniform_int_distribution<long long int> unid(-9999, 9999);

	char a = '9', b = '7';
	string s1{ "9" }, s2{ "-9" }, s3{ "-000000000000046" }, s4{ "-000" }, res, r;
	int n1 = 0, n2 = 0, real = 0;


	/*for (size_t i = 0; i < 1000; ++i)
	{
		n1 = unid(rng);
		n2 = unid(rng);
		real = n1 * n2;
		s1 = to_string(n1);
		s2 = to_string(n2);
		res = Karacuba_algorithm(s1, s2);
		cout << n1 << " * " << n2 << " = " << res << ", real: " << real << " equal? : " << (res == to_string(real)? "1":"0------------------") << endl;
	}*/

	
	cout << Karacuba_algorithm("-00000000000000000000000000000000000000000000", "73755555559999999999999999999999999999999552222114094946461604");
}