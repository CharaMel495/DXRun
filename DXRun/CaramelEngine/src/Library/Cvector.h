#pragma once

namespace CaramelEngine
{
    template <class T>
    class Cvector : public std::vector<T>
    {
    public:

        using std::vector<T>::vector;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="obj">配列に格納する要素</param>
        Cvector(const std::vector<T>& obj) : std::vector<T>(obj) {}

        /// <summary>
        /// 特定の条件を満たす最初の要素を返す関数
        /// </summary>
        /// <param name="func">条件式</param>
        std::optional<T> first(std::function<bool(const T&)> func) const
        {
            for (const auto& item : *this)
                if (func(item)) 
                    return item;

            return std::nullopt;
        }

        /// <summary>
        /// 特定の要件を満たす要素のみを取り出す関数
        /// </summary>
        /// <param name="func">要件定義</param>
        /// <returns>要件を満たす物だけにした配列</returns>
        Cvector where(std::function<bool(const T&)> func) const
        {
            Cvector result;
            for (const auto& item : *this)
                if (func(item)) result.push_back(item);
            result.shrink_to_fit();
            return result; // RVO (戻り値最適化) が効くので高速
        }

        template <typename Compare>
        Cvector& orderBy(Compare&& comp)
        {
            quiqSort(this->begin(), this->end(), std::forward<Compare>(comp));
            return *this;
        }


    private :

        /// <summary>
        /// 引数で貰ったイテレータをもとにクイックソートを行う関数
        /// 参考:https://qiita.com/rinse_/items/0c29a4d21131c88065ef
        /// </summary>
        /// <typeparam name="TIterator">イテレータテンプレート</typeparam>
        /// <param name="first">先頭イテレータ</param>
        /// <param name="last">末尾イテレータ</param>
        /// <param name="func">ソート条件</param>
        template <class TIterator, class Compare>
        inline void quiqSort(TIterator first, TIterator last, Compare&& comp)
        {
            if (last - first <= 1)
                return;

            auto i = first;
            auto j = last - 1;

            for (TIterator pivot = first;; ++i, --j)
            {
                while (comp(*i, *pivot))
                    ++i;

                while (comp(*pivot, *j))
                    --j;

                if (i >= j)
                    break;

                std::iter_swap(i, j);
            }
            quiqSort(first, i, comp);
            quiqSort(j + 1, last, comp);
        }

    };
}