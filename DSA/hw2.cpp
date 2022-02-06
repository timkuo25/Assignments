/*DSA HW2- Cryptocurrency Querying*/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
using namespace std;

class Data
{
private:
public:
  int date;
  string currency, exchange;
  float low, high;
  long long int cap;
};

bool cmp1(Data a, Data b)
{
  if (a.date < b.date)
    return true;
  else if (a.date > b.date)
    return false;
  else if (a.currency < b.currency)
    return true;
  else if (a.currency > b.currency)
    return false;
  else if (a.exchange < b.exchange)
    return true;

  return false;
}

bool cmp2(Data* a, Data* b)
{
  if (a->date < b->date)
    return true;
  else if (a->date > b->date)
    return false;
  else if (a->exchange < b->exchange)
    return true;

  return false;
}


int main(int argc, char** argv)
{
  FILE * pFile;
  char mystring [300];
  char * token;

  pFile = fopen(argv[1], "r");
  Data Test;
  vector<Data> data ; //多長?
  vector<Data*> data_by2;//vector of pointers
  vector<int> date; //create vectors for equal_range 
  vector<string> currency, exchange1, exchange2;
  int n = 0, cmd1, target;
  float pMin, pMax;
  string command, cmd2, cmd3;
  long long int total = 0;
  pair<vector<int>::iterator, vector<int>::iterator> bounds1;
  pair<vector<string>::iterator, vector<string>::iterator> bounds2, bounds3;

  while (fgets(mystring, 300, pFile) != NULL)
  {  
    token = strtok(mystring, "\t");
    Test.date = atoi(token);

    token = strtok(NULL, "\t");
    Test.currency = token;

    token = strtok(NULL, "\t");
    Test.exchange = token;

    token = strtok(NULL, "\t");
    Test.low = atof(token);

    token = strtok(NULL, "\t");
    Test.high = atof(token);

    token = strtok(NULL, "\n");
    Test.cap = atoll(token);

    n++;
    data.push_back(Test);
  }
  fclose(pFile);
  
  sort(data.begin(), data.end(), cmp1); //data sorted by cmp1
  

  for (int i = 0; i < data.size(); i++)
  {
    data_by2.push_back(&data[i]);
  }  
  sort(data_by2.begin(), data_by2.end(), cmp2); //sorted data by cmp2


  for (int i = 0; i < data.size(); i++)
  {
    date.push_back(data[i].date);
    currency.push_back(data[i].currency);
    exchange1.push_back(data[i].exchange);
    exchange2.push_back(data_by2[i]->exchange);
  }



  
  
  
  while (fgets(mystring, 300, stdin) != NULL)

  {
    token = strtok(mystring, "\t");
    command = token;

    if (command == "query") //如何判斷字串?
    {
      token = strtok(NULL, "\t");
      cmd1 = atoi(token);

      token = strtok(NULL, "\t");
      cmd2 = token;

      token = strtok(NULL, "\n");
      cmd3 = token;

      bounds1 = equal_range(date.begin(), date.end(), cmd1);
      if (bounds1.first == bounds1.second)
      {
        printf("none\n");
	    continue;
      }
      else
      {
        bounds2 = equal_range(currency.begin() + (bounds1.first - date.begin()), currency.begin() + (bounds1.second - date.begin()), cmd2);

        if (bounds2.first == bounds2.second)
        {
	      printf("none\n");
	      continue;
	    }

	    else
	    {
          bounds3 = equal_range(exchange1.begin() + (bounds2.first - currency.begin()), exchange1.begin() + (bounds2.second - currency.begin()), cmd3);

	      if (bounds3.first == bounds3.second)
	      {
	        printf("none\n");
	        continue;
	      }

	      else
	      {
	        target = bounds3.first - exchange1.begin();
	        printf("%.4f %.4f %lld\n", data[target].low, data[target].high, data[target].cap);
	      }    
	 
	    }
      }
      
    }
    
    
    
    else if (command == "price")
    {
      token = strtok(NULL, "\t");
      cmd2 = token;

      token = strtok(NULL, "\t");
      cmd1 = atoi(token);

      token = strtok(NULL, "\n");
      cmd3 = token;

      bounds1 = equal_range(date.begin(), date.end(), cmd1);
      
      if (bounds1.first == bounds1.second)
      {
        printf("none\n");
	    continue;
      }
      else
      {
        bounds2 = equal_range(currency.begin() + (bounds1.first - date.begin()), currency.begin() + (bounds1.second - date.begin()), cmd3);

        if (bounds2.first == bounds2.second)
        {
	      printf("none\n");
	      continue;
	    }
	    
	    else
	    {
	      if (cmd2[1] == 'i')
	      {
		    target = bounds2.first - currency.begin();
		    pMin = data[target].low;
	        for (int i = bounds2.first - currency.begin(); i < bounds2.second - currency.begin(); i++)
	        {
              if (data[i].low < pMin)
              {
                pMin = data[i].low;
                target = i;
			  }
		    }
	        cout << fixed << setprecision(4) << data[target].low << endl;
	      }
	      else
          {
          	target = bounds2.first - currency.begin();
		    pMax = data[target].high;
	        for (int i = (bounds2.first - currency.begin()); i < bounds2.second - currency.begin(); i++)
	        {
              if (data[i].high > pMax)
              {
                pMax = data[i].high;
                target = i;
			  }
		    }
	        cout << fixed << setprecision(4) << data[target].high << endl;
		  }
          
		}
	  }
	} 
      


    else if (command == "cap")
    {
      token = strtok(NULL, "\t");
      cmd1 = atoi(token);

      token = strtok(NULL, "\n");
      cmd2 = token;

      bounds1 = equal_range(date.begin(), date.end(), cmd1);
    
      if (bounds1.first == bounds1.second)
      {
        printf("none\n");
	    continue;
      }
      
      else
      {
        bounds2 = equal_range(exchange2.begin() + (bounds1.first - date.begin()), exchange2.begin() + (bounds1.second - date.begin()), cmd2);
        
        if (bounds2.first == bounds2.second)
        {
	      printf("none\n");
	      continue;
	    }
	    
	    else
	    {
	       for (int i = (bounds2.first - exchange2.begin()); i < (bounds2.second - exchange2.begin()); i++)
	         total += data_by2[i]->cap;
	      printf("%lld\n", total);
	      total = 0;
	    }
	  }
    }
    
    else if (command == "exit")
	  break;
  }
  return 0;
}
