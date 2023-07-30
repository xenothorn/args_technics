#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
              return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
          });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}
/*
// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> a;
    vector<Token> tv;
    bool end_of_sentence = false;
    for (const auto& t: tokens)
    {
        if (t.IsEndSentencePunctuation()) 
        {
            if (!end_of_sentence) end_of_sentence = true;
            tv.push_back(move(t));
        }
        else
        {
            if (end_of_sentence)
            {
                end_of_sentence = false;
                a.push_back(move(tv));
                tv.clear();            
            }
            tv.push_back(move(t));  
        }
    }
    a.push_back(move(tv));
    return a;
}*/
// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token>&& tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> a;
    vector<Token> tv;
    bool end_of_sentence = false;
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if ((*it).IsEndSentencePunctuation()) 
        {
            if (!end_of_sentence) end_of_sentence = true;
            tv.push_back(move(*it));
        }
        else
        {
            if (end_of_sentence)
            {
                end_of_sentence = false;
                a.push_back(move(tv));
                tv.clear();            
            }
            tv.push_back(move(*it));  
        }
    }
    a.push_back(move(tv));
    return a;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

struct TestTokenNoCopy :public TestToken {
    TestTokenNoCopy(const TestTokenNoCopy& other) = delete;
    TestTokenNoCopy(TestTokenNoCopy&& other) = default;
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

ostream& operator<<(ostream& stream, const TestTokenNoCopy& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}}));

    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}}));

    assert(SplitIntoSentences(vector<TestToken>(
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true}}))
           == vector<Sentence<TestToken>>({
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
               {{"Without"s}, {"copies"s}, {"."s, true}},
           }));
}


void TestSplittingNoCopy(){
    vector<TestTokenNoCopy> nct;
    nct.push_back({"send"s});
    nct.push_back({"send"s});
    nct.push_back({"."s, true});

    vector<TestTokenNoCopy> ncc;
    ncc.push_back({"send"s});
    ncc.push_back({"send"s});
    ncc.push_back({"."s, true});

    vector<Sentence<TestTokenNoCopy>> st = move(SplitIntoSentences(move(nct)));
    assert (st[0] == ncc) ;
}


int main() {
    TestSplittingNoCopy();
    TestSplitting();
}