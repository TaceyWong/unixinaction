#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <ctime>

using namespace std;
/*
	单词字典类 
*/
class WordDictory
{
public :

	/*
		判断字符串 str 的内容是一个单词
		单词的定义是：字符串的内容组成为字符 'A'~'Z' , 'a'~'z' 
	*/
	static bool isWord(string str)
	{
		for(int i=0;i<str.length();i++)
		{
			if((str[i]>='A'&&str[i]<='Z')||(str[i]>='a'&&str[i]<='z'))
			{
				continue;	
			}
			else
			{
				return false; 
			}
		}
		return true;
	}
	/*
		比较两个单词，可以指定是否大小写敏感 
	*/
	static int compareWords(string strA,string strB,bool isCaseSensitive = false)
	{
		int i=0;
		for(;i<strA.length() && i<strB.length();i++)
		{
			if(isCaseSensitive)
			{
				if(strA[i]!=strB[i])
				{
					return (strA[i] - strB[i])>0 ? 1 : -1;
				}
			}
			else
			{
				char a = (strA[i]>='a' && strA[i]<='z') ? (strA[i] - ('a'-'A')) : strA[i];
				char b = (strB[i]>='a' && strB[i]<='z') ? (strB[i] - ('a'-'A')) : strB[i];
				
				if(a != b)
				{
					return (a-b)>0 ? 1 : -1;
				}
			}
		}	
		
		if(i == strA.length() && i == strB.length())
		{
			return 0;
		}
		else if(i == strA.length())
		{
			return -1;
		}
		else if(i== strB.length())
		{
			return 1;
		}
	}
	
	// 默认样式，不对单词做任何修改 
	static const int SENTENCE_FORMAT_DEFAULT = 0;
	// 第一个单词的首字符大写 
	static const int SENTENCE_FORMAT_WITH_HEAD_WORD = 1;
	// 所有单词的所有字符大写 
	static const int SENTENCE_FORMAT_ALL_CHAR_TO_UPPERCASE = 2;
	// 所有单词的所有字符小写
	static const int SENTENCE_FORMAT_ALL_CHAR_TO_LOWERCASE = 3;
	
	/*
		将 wordsBuffer 单词列表里面的单词，
		按顺序连接组成一个句子，
		使用 seperator 作为单词间的分隔符号s。 
		
		isReversed 标示是否要反转（从右到左阅读顺序） ，默认不反转。 
		
		每个单词根据 formate 样式进行处理； 
		默认的分隔符是一个空格;
		句子末尾没有分隔符号。 
	*/
	static string makeSentence(list<string> &wordsBuffer,string seperator = " ",int format = SENTENCE_FORMAT_DEFAULT,bool isReversed=false)
	{
		string str ="";
	    for(list<string>::iterator i = wordsBuffer.begin(); i != wordsBuffer.end(); i++ )
	    {
	    	str += (*i);
	    	// 添加单词之间分隔符，末尾无空格 
	    	i++;
	    	if(i!=wordsBuffer.end())
	    	{
	    		str += seperator;
	    	}
	    	i--;
	    }
	    
		// 格式：首字符大写 
		if(format == SENTENCE_FORMAT_WITH_HEAD_WORD)
		{
			str[0] = (str[0]>='a' && str[0]<='z') ? (str[0] - ('a'-'A')) : str[0];
		} 
		// 格式：所有单词的所有字符大写 
		else if(format == SENTENCE_FORMAT_ALL_CHAR_TO_UPPERCASE)
		{
			for(int i=0;i<str.length();i++)
			{
				str[i] = (str[i]>='a' && str[i]<='z') ? (str[i] - ('a'-'A')) : str[i];
			}
		}
		// 格式：所有单词的所有字符小写 
		else if(format == SENTENCE_FORMAT_ALL_CHAR_TO_LOWERCASE)
		{
			for(int i=0;i<str.length();i++)
			{
				str[i] = (str[i]>='A' && str[i]<='Z') ? (str[i] + ('a'-'A')) : str[i];
			}			
		}
		
		// 反转 
		if(isReversed) 
		{
			string reversedStr = "";
			for(int i=0;i<str.length();i++)
			{
				reversedStr =  str[i] + reversedStr;
			}			
			
			str = reversedStr;
		}
		
		return str;			
	} 
	
	/*
		计算单词 word 在 wordsBuffer 中出现的次数 
	*/
	static int getWordUsedTime(string word,list<string> & wordsBuffer)
	{
		int t = 0;
		
	    for(list<string>::iterator i = wordsBuffer.begin(); i != wordsBuffer.end(); i++ )
	    {
			if(word == (*i))
			{
				t++;
			}
	    }
		return t;		
	}
		
	/*	
		构造函数 
		fileName :	单词文件的路径 
	*/
	WordDictory(string wordDictoryFileName)
	{
		this->wordDictoryFileName = wordDictoryFileName;
		this-> minWordLength = 0;
		this-> maxWordLength = 0;
			
		ifstream fin(this->wordDictoryFileName.c_str()) ;
		if(fin)
		{
			while(!fin.eof())
			{
				string str = "";
				fin >> str;
				// 判断读入的是否是单词 
				if(isWord(str))
				{
					this->wordDictory.push_back(str);
					// 统计最短单词长度 
					this-> minWordLength = (this-> minWordLength==0 || this-> minWordLength > str.length() )
											?  str.length() : this-> minWordLength ;
					// 统计最长单词长度 						
					this-> maxWordLength = (this-> maxWordLength==0 || this-> maxWordLength < str.length() )
											?  str.length() : this-> maxWordLength ;											
				}
			}
			fin.close(); 
			
			// 单词字典排序 
			sort(this->wordDictory.begin(),this->wordDictory.end());
		}
		else
		{
			cerr << "无法打开单词文件：" << this->wordDictoryFileName <<endl;
		}		
	}
	
	/*
		取得单词字典 
	*/
	vector<string>& getWordDictory()
	{
		return this->wordDictory;
	}
	/*
		取得单词字典中最短单词长度 
	*/
	int getMinWordLength()
	{
		return this->minWordLength;
	}
	/*
		取得单词字典中最长单词长度 
	*/	
	int getMaxWordLength()
	{
		return this->maxWordLength;
	}
		
	/*
		判断能否通过字典里面的单词组合，得到 subfix 这样的一个结尾 
		
		subfix			： 一个要用单词组合出来的结尾 
		maxUseWordsNum	： 组合时能使用的最多单词数目 
		availablePerWordUsedTime	：	组合时每个单词能使用的做多次数（哈希表） 
		isCaseSensitive	： 是否大小写敏感（默认不敏感）
		 
	*/
	bool canMakeTheSubfix(string subfix,int maxUseWordsNum,map<string,int> &availablePerWordUsedTime,bool isCaseSensitive = false)
	{
		// 递归终止条件：可以使用的单词数目已经达到上限 
		if(maxUseWordsNum == 0)
		{
			return false;
		} 
		// 枚举字典里面的每个单词 
		for(int i=0;i<this->wordDictory.size();i++) 
		{
			string word = this->wordDictory[i];
			// 判断当前单词是否还能使用（使用次数没有达到限制） 
			if(availablePerWordUsedTime[word] > 0)
			{
				// 当前单词 word 不比要找的后缀 subfix 短 
				if(word.length() >= subfix.length())
				{					
					// 计算当前单词和要找的后缀的长度差 
					int subLenght = word.length() - subfix.length();					
					// 判断当前单词 word 是否以后缀 subfix 结尾，并作为结果返回 
					if(compareWords(word.substr(subLenght) , subfix , false) == 0)
					{
						return true;
					}
				}
				// 当前单词 word 比要找的后缀 subfix 短 
				else
				{
					// 计算要找的后缀 subfix 和当前单词 word 的长度差 
					int subLenght = subfix.length() - word.length();
					
					// 判断后缀 subfix 是否以当前单词 word 结尾
					if(compareWords(subfix.substr(subLenght) , word) == 0)
					{
						// 标记当前单词被使用一次 
						availablePerWordUsedTime[word]--;
						/*
						 	递归
						 	递归时的后缀，为在当前后缀 subfix 的末尾， 减去当前单词 word ，剩下的部分
							递归时可以使用的最多单词数 maxUseWordsNum 减一   
						*/  
						bool can = canMakeTheSubfix(subfix.substr(0,subLenght),maxUseWordsNum-1,availablePerWordUsedTime,false);
						// 回溯 
						availablePerWordUsedTime[word]++;
						if(can)
						{
							return true;
						}
					}
				}
			}
		}
		
		return false;
	}	

private:
	string wordDictoryFileName;
	vector<string> wordDictory;
	
	int minWordLength;
	int maxWordLength;
};

/*
	产生单词回文类 
*/
class WordPalindrome
{
public:
	// 默认的单词文件 
	static const string DEFAULT_WORDS_FILE_NAME;
	// 默认的生成回文保存位置 
	static const string DEFAULT_WORDS_PALINDROM_FILE_NAME;
	
	// 表示无限的特殊数字，因为这里的数字取值都是正整数，用一个负数表示无限 
	static const int INIFINITE = -1;	
	
	/*
		构造函数 
		minWordsUsedNum			：	指定产生回文最少使用的单词数，默认 1 个单词 
		maxWordsUsedNum 		：	指定产生回文最多使用的单词数，默认没有上限（值为-1）
		maxPerWordUsedTime		：	每个单词在回文中能出现的最多次数，默认没有上限（值为-1）	
		
		neededWordPalindromeNum	: 	指定需要产生的回文总数。
									当产生的回文数到达这个数目，
									或者指定使用的单词数[minWordsUsedNum,maxWordsUsedNum]范围内所有回文都已经产生,但是还没到达这个数目，
									停止继续产生回文。
									
									默认产生尽可能多数目的回文（值为-1）	
		isCaseSensitive			：	大小写是否敏感，默认不敏感（false）									 
		wordDictoryFileName		：	指定产生回文是用的单词文件，默认为程序同目录下的 words.txt 文件 
		
		wordPalindromeFileName	:	指定的产生的回文保存文件，默认为程序同目录下的 wordPalindrome.txt 文件 
	*/
	WordPalindrome(	int minWordsUsedNum=1,int maxWordsUsedNum=INIFINITE,
					int maxPerWordUsedTime = INIFINITE,
					int neededWordPalindromeNum = INIFINITE,
					bool isCaseSensitive = false,
					string wordDictoryFileName=DEFAULT_WORDS_FILE_NAME,
					string wordPalindromeFileName=DEFAULT_WORDS_PALINDROM_FILE_NAME)
	{
		this->minWordsUsedNum = minWordsUsedNum;
		this->maxWordsUsedNum = maxWordsUsedNum;
		
		this->maxPerWordUsedTime = maxPerWordUsedTime; 
		this->neededWordPalindromeNum = neededWordPalindromeNum;
			 
	 	this->isCaseSensitive = isCaseSensitive;
	 	
	 	this->wordDictoryFileName = wordDictoryFileName;
		this->wordDictory = new WordDictory(this->wordDictoryFileName);
		
		this->wordPalindromeFileName = wordPalindromeFileName;
	}
	~WordPalindrome()
	{
		if(this->wordDictory)
		{
			delete(this->wordDictory);
		}
	}
	/*
		开始产生回文 
	*/ 
	void execute()
	{
		// 记录开始运行时间 
		this->startExecuteTime = time(NULL);
		
		// 统计当前已经产生的回文数目 
		int currentWordPalindromeNum = 0;
		
		// 打开输出产生回文的文件 
		ofstream foutWordPalindrome (this->wordPalindromeFileName.c_str());
		
		for(
			int useWordsNum = this->minWordsUsedNum; 
			
			// 使用单词数限制 
			(useWordsNum < this->maxWordsUsedNum ||  this->maxWordsUsedNum == INIFINITE) 
			&& 
			// 产生回文总数限制 
			(currentWordPalindromeNum < this->neededWordPalindromeNum || this->neededWordPalindromeNum == INIFINITE)
			;
			
			useWordsNum ++ 
			) 
		{
			list<string> wordsBuffer;
			getWordPalindrome(useWordsNum,currentWordPalindromeNum,wordsBuffer,foutWordPalindrome); 
		}
		// 关闭文件 
		foutWordPalindrome.close();	
		
		cout << "一共产生了：" << currentWordPalindromeNum << " 个回文句" 
			 << "用时：" << (time(NULL) - this->startExecuteTime)<< "秒。" <<endl; 
		
	}
public:
	/*
		判断字符串 str 是否回文结构 
		
		isCaseSensitive 指定是否大小写敏感,默认不区分大小写 
	*/
	static bool isPalindrome(string str,bool isCaseSensitive = false)
	{    
		for(int i=0,j=str.length()-1;i<j;i++,j--)
		{
			char a = (str[i]>='a' && str[i]<='z') ? (str[i] - ('a'-'A')) : str[i];
			char b = (str[j]>='a' && str[j]<='z') ? (str[j] - ('a'-'A')) : str[j];
			
			if((isCaseSensitive && str[i] != str[j]) || (!isCaseSensitive && a!=b ))
			{
				return false;		
			}
		}
		return true;
	}
		
	/*
		判断 wordsBuffer 单词列表里面的单词，按顺序连接是否一个回文 
	*/
	static bool isPalindrome(list<string> & wordsBuffer,bool isCaseSensitive = false)
	{
		// 顺序连接单词 
		string str = WordDictory::makeSentence(wordsBuffer,"",WordDictory::SENTENCE_FORMAT_DEFAULT);
	    return isPalindrome(str,isCaseSensitive); 
	}

private:	
	/*
		产生使用  useWordsNum 个单词的回文数并输出
		 
		useWordsNum					:	产生回文使用的单词个数 
		currentWordPalindromeNum	:	当前已经产生的回文数目 
		wordsBuffer					:	当前选中的用来产生回文的单词，供递归使用 
	*/
	void getWordPalindrome(int useWordsNum,int ¤tWordPalindromeNum,list<string> &wordsBuffer,ostream& fout)
	{
		// 已经产生所需要数目的回文，返回 
		if(this->neededWordPalindromeNum != INIFINITE && currentWordPalindromeNum == this->neededWordPalindromeNum )
		{
			return ;
		}
		
		//	已经枚举了 useWordsNum 个单词
		if(useWordsNum == wordsBuffer.size())
		{
			// 进行是否能构成回文的判断 
			if(isPalindrome(wordsBuffer,this->isCaseSensitive))
			{
				// 当前已经产生的回文数目计数器加一 
				currentWordPalindromeNum ++;
				// 目前挑选的单词组成一个句子 
				string str = WordDictory::makeSentence(wordsBuffer," ",WordDictory::SENTENCE_FORMAT_WITH_HEAD_WORD);
				
				// 输出到屏幕 
			    cout << "第 " << currentWordPalindromeNum << " 个：" 
					<< str << endl;
					
   				// 输出到文件 
   				if(fout)
   				{
			    	fout << "第 " << currentWordPalindromeNum << " 个：" 
						 << str << endl;
		   		} 
   				
			    // debug 输出产生回文句子 "Was it a car or a cat I saw" 使用的时间 
			    if(str ==  "Was it a car or a cat I saw")
			    {
			    	cout << "oh,we find it!"<<endl; 
					system("pause");
    			}
			}
			return ;
		}
		
		// 枚举当前可以使用的单词 
		for(int i=0;i<this->wordDictory->getWordDictory().size();i++)
		{
			string word = this->wordDictory->getWordDictory()[i];
			
			// 每个单词的使用次数有限制 
			if(this->maxPerWordUsedTime == INIFINITE || WordDictory::getWordUsedTime(word,wordsBuffer) < this->maxPerWordUsedTime)
			{
				// 末尾添加一个单词 
				wordsBuffer.push_back(word);
					
				// 剪枝，判断添加当前枚举的单词后，是否可以在接下来构造出回文 
				if(hasFollowWords(useWordsNum,wordsBuffer))
				{
					// 递归 
					getWordPalindrome(useWordsNum,currentWordPalindromeNum,wordsBuffer,fout); 		
				} 
				
				// 回溯，删除末尾单词 
				wordsBuffer.pop_back();					
			} 
		}
	}
	
	/*
		递归函数 getWordPalindrome 的剪枝函数 
		
		useWordsNum					:	产生回文使用的单词个数 
		currentWordPalindromeNum	:	当前已经产生的回文数目 	
	
	*/
	bool hasFollowWords(int useWordsNum,list<string> & wordsBuffer)
	{
		/*
		 可以直接返回 true 不剪枝，会很慢。用于比较剪枝和不剪枝的效果。
		  
		 测试数据证明，
		 	1）不使用下面的 剪枝1 和 剪枝2 ，也就是完全的暴力枚举单词的组合情况，
			 程序运行需要730多秒，相当于 11 多分钟 
		  	
		  	2）使用下面的 剪枝1 和 剪枝2 ，程序运行需要3~4秒 
		*/ 
		// return true;
		
		string strPartA = WordDictory::makeSentence(wordsBuffer,"",WordDictory::SENTENCE_FORMAT_DEFAULT);
		int lengthPartA = strPartA.length();
		int	minLenghtPartB = (useWordsNum - wordsBuffer.size()) * this->wordDictory->getMinWordLength();
		int	maxLenghtPartB = (useWordsNum - wordsBuffer.size()) * this->wordDictory->getMaxWordLength();
		
		/*
			剪枝1：
				已经挑选的单词连接成的长度  lengthPartA ，
				小于等于后续单词能连接成的最小程度。 
				
				那么后续单词连接成的字符串，必须以已经挑选出来单词的回文，作为后缀。 
		*/
		if(lengthPartA <= minLenghtPartB)
		{
			// 反转的字符串 
			string reversedStrPartA = WordDictory::makeSentence(wordsBuffer,"",WordDictory::SENTENCE_FORMAT_DEFAULT,true);
			// 字典中每个单词可以继续使用的次数 
			map<string,int> availablePerWordUsedTime;
			for(int i=0;i<this->wordDictory->getWordDictory().size();i++)
			{
				string word = this->wordDictory->getWordDictory()[i];
				int	t = this->maxPerWordUsedTime - WordDictory::getWordUsedTime(word,wordsBuffer);
				
				availablePerWordUsedTime[word] = t; 
			}
			
			int maxUseWordsNum = useWordsNum - wordsBuffer.size();
			
			return this->wordDictory->canMakeTheSubfix(reversedStrPartA,maxUseWordsNum,availablePerWordUsedTime);
		}
		/*
			剪枝2：
				已经挑选的单词连接成的长度  lengthPartA ，
				大于等于后续单词能连接成的最大程度。 
				
				那么当前单词连接成的字符串，后缀必须有一部分能自己组成回文
				这个剪枝很强大，能优化很多。				 
		*/		
		else if(lengthPartA >= maxLenghtPartB)
		{
			for(int len=minLenghtPartB ; len<=maxLenghtPartB ; len++)
			{
				if(isPalindrome(strPartA.substr(len),this->isCaseSensitive))
				{
					return true; 
				}
			}
			return false;	
		}
		// TODO 可以加入其他你能想到的剪枝方法 
		else
		{
			return true; 
		}
	}
		
private:
	// 可以使用的单词数目范围 
	int minWordsUsedNum;
	int maxWordsUsedNum;
	
	// 每个单词可以在回文中出现的最多次数 
	int maxPerWordUsedTime;
	// 需要的回文句子总数 
	int neededWordPalindromeNum;
	
	// 回文中大小写是否敏感 
	bool isCaseSensitive;
	
	// 产生回文的单词文件 
	string wordDictoryFileName;
	class WordDictory* wordDictory;
	
	// 保存产生回文的文件 
	string wordPalindromeFileName;
	
	time_t startExecuteTime;
};

/*
	测试时 words.txt 的内容：
		a cat car I it or saw was  
*/
const string WordPalindrome::DEFAULT_WORDS_FILE_NAME = "words.txt";
const string WordPalindrome::DEFAULT_WORDS_PALINDROM_FILE_NAME = "wordPalindrome.txt";

int main(int argc, char *argv[])
{
	/*
	 使用全部参数构造 WordPalindrome 类 
	 实际上 WordPalindrome 类有默认构造函数 WordPalindrome() 
	*/ 
	WordPalindrome wordPalindrome
	(
		// 使用1~10个单词
		1,10,
		// 每个单词最多用4次
		4,
		// 产生尽量多的回文 
		WordPalindrome::INIFINITE,
		// 回文大小写不敏感 
		false,
		// 产生回文的单词文件 
		WordPalindrome::DEFAULT_WORDS_FILE_NAME
		// 产生的回文保存到文件 
	);
	wordPalindrome.execute();
	return 0;
}
