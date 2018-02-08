#pragma once

#include <vector>
#include <algorithm> // std::find, std::remove
#include <stdexcept>

namespace UTILS_MODULE_NS
{
    /**
 * \brief Class, controlling unique elements merging.
 * Shall be used in GameObject, Scene and GameObjectController.
 */
    template<
        typename T,
        typename Container=std::vector<T>
    >
    class UniqueMergeContainer
    {
    public:
        typedef typename Container::const_iterator const_iterator;
        typedef typename Container::iterator iterator;
        typedef typename Container::size_type size_type;

        /**
         * @brief Default constructor creates no elements.
         * @return
         */
        UniqueMergeContainer() :
            m_removable(),
            m_added(),
            m_current()
        {

        }

        /**
         * @brief Copy constructor. It's copying states.
         * @param container Copying object.
         */
        UniqueMergeContainer(const UniqueMergeContainer<T, Container> &container) :
            m_removable(container.m_removable),
            m_added(container.m_added),
            m_current(container.m_current)
        {

        }

        /**
         * @brief Move constructor. It's just moving container data.
         * @param container Movable container.
         */
        UniqueMergeContainer(UniqueMergeContainer<T, Container>&& container) :
            m_removable(std::move(container.m_removable)),
            m_added(std::move(container.m_added)),
            m_current(std::move(container.m_current))
        {

        }

        /**
         * @brief Destructor.
         */
        ~UniqueMergeContainer() = default;

        /**
         * @brief Adding element to add queue. This elements
         * will be added to main container after merge. If they
         * would not be removed by UniqueMergeVector::remove.
         * @param e New Element.
         */
        void add(T e)
        {
            if (std::find(m_added.begin(), m_added.end(), e) != m_added.end())
                return;

            if (std::find(m_current.begin(), m_current.end(), e) != m_current.end())
                return;

            m_added.emplace_back(std::move(e));
        }

        /**
         * @brief Adding element to removable queue. It will remove
         * new elements from add queue and from current queue after merge.
         * @param e Removable element.
         */
        void remove(T e)
        {
            if ((std::find(m_added.begin(), m_added.end(), e) == m_added.end() ||
                 std::find(m_current.begin(), m_current.end(), e) == m_current.end()) &&
                std::find(m_removable.begin(), m_removable.end(), e) != m_removable.end())
            {
                return;
            }

            m_removable.emplace_back(std::move(e));
        }

        /**
         * @brief Method to merge all elements. At the beginning it will
         * remove elements from add queue and from current elements container.
         * After that left new elements will be added to current elements container.
         */
        void merge()
        {
            // Removing elements from new one and from current
            typename Container::iterator addedNewEndIterator = m_added.end();
            typename Container::iterator currentNewEndIterator = m_current.end();

            for (auto&& el : m_removable)
            {
                addedNewEndIterator   = std::remove(m_added.begin()  , addedNewEndIterator  , el);
                currentNewEndIterator = std::remove(m_current.begin(), currentNewEndIterator, el);
            }

            m_added.erase(addedNewEndIterator, m_added.end());
            m_current.erase(currentNewEndIterator, m_current.end());

            // Adding new elements to current
            for (auto&& el : m_added)
            {
                m_current.emplace_back(std::move(el));
            }

            m_added.clear();
            m_removable.clear();
        }

        /**
         * @brief Method to clear added and removed queues.
         */
        void clearState()
        {
            m_removable.clear();
            m_added.clear();
        }

        /**
         * @brief Copy operator.
         * @param rhs Right hand container.
         * @return Reference to this container.
         */
        UniqueMergeContainer<T, Container>& operator=(const UniqueMergeContainer<T, Container>& rhs)
        {
            m_current   = rhs.m_current;
            m_removable = rhs.m_removable;
            m_added     = rhs.m_added;

            return *this;
        };

        /**
         * @brief Move operator.
         * @param rhs Right hand container.
         * @return Reference to this container.
         */
        UniqueMergeContainer<T, Container>& operator=(UniqueMergeContainer<T, Container>&& rhs)
        {
            m_current   = std::move(rhs.m_current);
            m_removable = std::move(rhs.m_removable);
            m_added     = std::move(rhs.m_added);

            return *this;
        };

        /**
         * @brief Indexed operator with referenced return.
         * @param i Number of element.
         * @return References element instance.
         */
        T& operator[](int i)
        {
            if (i < 0 || i >= size())
            {
                throw std::out_of_range("Size: " + std::to_string(size()) + ", Index: " + std::to_string(i));
            }

            return m_current[i];
        }

        /**
         * @brief Const index operator.
         * @param i Number of element.
         * @return Copy of element instance.
         */
        T operator[](int i) const
        {
            if (i < 0 || i >= size())
            {
                throw std::out_of_range("Size: " + std::to_string(size()) + ", Index: " + std::to_string(i));
            }

            return m_current[i];
        }

        /**
         * @brief Method to get number of current elements.
         * @return Number of current elements.
         */
        size_type size() const
        {
            return m_current.size();
        }

        /**
         * @brief Method to get number of added elements.
         * @return Size.
         */
        size_type numberOfAdded() const
        {
            return m_added.size();
        }

        /**
         * @brief Method to get begin const iterator of current container.
         * @return Const iterator.
         */
        const_iterator begin() const
        {
            return m_current.begin();
        }

        /**
         * @brief Method to get end const iterator of current container.
         * @return Const iterator.
         */
        const_iterator end() const
        {
            return m_current.end();
        }

        /**
         * @brief Method to get begin iterator of current container.
         * @return Iterator.
         */
        iterator begin()
        {
            return m_current.begin();
        }

        /**
         * @brief Method to get end iterator of current container.
         * @return Iterator.
         */
        iterator end()
        {
            return m_current.end();
        }

        /**
         * @brief Method to get begin iterator of addable container.
         * @return Iterator.
         */
        iterator addedBegin()
        {
            return m_added.begin();
        }

        /**
         * @brief Method to get end iterator of addable container.
         * @return Iterator.
         */
        iterator addedEnd()
        {
            return m_added.end();
        }

        /**
         * @brief Method to get const begin iterator of addable container.
         * @return Iterator.
         */
        iterator addedBegin() const
        {
            return m_added.begin();
        }

        /**
         * @brief Method to get const end iterator of addable container.
         * @return Iterator.
         */
        iterator addedEnd() const
        {
            return m_added.end();
        }

    private:
        Container m_removable;
        Container m_added;
        Container m_current;
    };
}