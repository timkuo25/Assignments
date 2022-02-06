/*DSA HW3- Chess Game Tree and Alpha-Beta Pruning*/

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <unordered_map> 
using namespace std;

int list[12][5] = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}, 
{16, 17, 18, 19, 20}, {21, 22, 23, 24, 25}, {1, 6, 11, 16, 21}, 
{2, 7, 12, 17, 22}, {3, 8, 13, 18, 23}, {4, 9, 14, 19, 24}, 
{5, 10, 15, 20, 25}, {1, 7, 13, 19, 25}, {5, 9, 13, 17, 21}};

unordered_map<long long, int>record;


long long place_O (long long board, int a, int b)
{
	board |= (1ll << (51 - 2*a));
	board |= (1ll << (51 - 2*b));
	
	return board;
}

long long place_X (long long board, int a, int b)
{
	board |= (1ll << (50 - 2*a));
	board |= (1ll << (50 - 2*b));
	
	return board;
}

long long last_move (long long board, int a, int b, int c)
{
	board |= (1ll << (50 - 2*a));
	board |= (1ll << (50 - 2*b));
	board |= (1ll << (50 - 2*c));
	
	return board;
}

vector<long long> all_next_board (long long board, int count)
{
	vector <long long> result(0);
	vector <int> blank(0);
	
	for (int i = 1; i < 26; i++)
	{
		if (((board >> (50 - 2*i)) & 3) == 0)
		{
			blank.push_back(i);
		}
	}
	int left = count - 2, right = count - 1;
	
	if (((count - 1) % 4) == 0) // O's turn
	{
		for (right; right > 0; right--)
		{
			left = right - 1;
			for(left; left > -1; left--)	
			{
				result.push_back(place_O(board, blank[left], blank[right]));
			}
			
		}
	}
	
	else // X's turn
	{
		if (count == 3)
		{
			result.push_back(last_move(board, blank[0], blank[1], blank[2]));
		}
		
		else
		{
			for (right; right > 0; right--)
			{
				left = right - 1;
				for(left; left > -1; left--)	
				{
					result.push_back(place_X(board, blank[left], blank[right]));
				}
			
			}
		}
	}
	
	return result;
}

int evaluate (long long board)
{
	int x = 0, o = 0;

	
	vector<int> o_vector, x_vector;
	
	for (int i = 1; i < 26; i++)
	{
		if (((board >> (50 - 2*i)) & 3) == 2)
			o_vector.push_back(i);
		else
			x_vector.push_back(i);
	}
	
	for (int i = 0; i < 12; i++)
	{
		int oCount = 0, xCount = 0; 
		for (int j = 0; j < 5; j++)
		{
			if (find(o_vector.begin(), o_vector.end(), list[i][j]) != o_vector.end())
				oCount += 1;
			if (find(x_vector.begin(), x_vector.end(), list[i][j]) != x_vector.end())
				xCount += 1;
		}
		
		if (oCount >= 4)
			o += 1;
		if (xCount >= 4)
			x += 1;
	}
	
	if (o > x)
		return 1;
	else if (o < x)
		return -1;
	else
		return 0;
}

int who_win(long long board, int count)
{
	if (record.find(board) != record.end())
	{
		unordered_map<long long, int>::const_iterator got = record.find(board);
		return got -> second;
	}

	
	if (count == 1)
	{	
		record.insert({board, evaluate(board)});
		return evaluate(board);
	}
		
	if (((count - 1) % 4) == 0) // O's turn
	{
		vector<long long> next = all_next_board(board, count);
		int result = -1;
		bool draw = false;
		
		for (int i = 0; i < next.size(); i++)
		{
			if (draw == false)
			{
				int win = who_win(next[i], count - 2);
				record.insert({next[i], win});
				if (win == 0)
				{
					result = 0;
					draw = true;
					continue;
				}
				else if (win == 1)
				{
					result = 1;
					break;
				}
			}
			
			if (draw == true)
			{
				vector<long long> next2 = all_next_board(next[i], count - 2);
				result = 1;

				for (int j = 0; j < next2.size(); j++)
				{
					int win2 = who_win(next2[j], count - 4);
					record.insert({next2[j], win2});
					if (win2 == -1 or win2 == 0)
					{
						result = 0;
						break;
					}
				}
				
				if (result == 1)
					break;
			}

		}
		
		return result;
	}
	
	if (count == 3)
	{
		vector<long long> next = all_next_board(board, count);
		int result = 1;
		
		for (int i = 0; i < next.size(); i++)
		{
			int win = who_win(next[i], count - 2);
			record.insert({next[i], win});
			if (win == 0)
				result = 0;
			if (win == -1)
			{
				result = -1;
				break;
			}
		}
		
		return result;
	}
	
	else
	{
		vector<long long> next = all_next_board(board, count);
		int result = 1;
		bool draw = false;
		
		for (int i = 0; i < next.size(); i++)
		{
			if (draw == false)
			{
				int win = who_win(next[i], count - 2);
				record.insert({next[i], win});
				if (win == 0)
				{
					result = 0;
					draw = true;
					continue;
				}
				else if (win == -1)
				{
					result = -1;
					break;
				}
			}
			
			if (draw == true)
			{
				vector<long long> next2 = all_next_board(next[i], count - 2);
				result = -1;

				for (int j = 0; j < next2.size(); j++)
				{
					int win2 = who_win(next2[j], count - 4);
					record.insert({next2[j], win2});
					if (win2 == 1 or win2 == 0)
					{
						result = 0;
						break;
					}
				}
				
				if (result == -1)
					break;
				
			}

		}
		
		return result;
	}
}





int main()
{
	char temp[5];
	int set = 0;

	scanf("%d", &set);
	for (int i = 0; i < set; i++) // 處理set個盤面
	{
		long long board = 0;
		for (int j = 0; j < 4; j++)
		{
			scanf("%s", temp);
			for(int k = 0; k < 5; k++)
			{
				if (temp[k] == 'O')
					board += 2;
				else if (temp[k] == 'X')
					board += 1;
				else
					board += 0;
				board <<= 2;
			}
		}

		scanf("%s", temp);
		for (int k = 0; k < 4; k++)
		{
			if (temp[k] == 'O')
				board += 2; 
			else if (temp[k] == 'X')
				board += 1;
			else
				board += 0;
			board <<= 2;
		}
		if (temp[4] == 'O')
			board += 2; 
		else if (temp[4] == 'X')
			board += 1;
		else
			board += 0; //????J????


		int count = 0;

		for (int i = 1; i < 26; i++)
		{
			if (((board >> (50 - 2*i)) & 3) == 0)
			{
				count += 1;
			}
		}

		if (count == 0)
			count = 1;
		
		int result = who_win(board, count);
		if (result == 1)
			printf("O win\n");
		else if (result == -1)
			printf("X win\n");
		else
			printf("Draw\n");
	}

  return 0;
}

