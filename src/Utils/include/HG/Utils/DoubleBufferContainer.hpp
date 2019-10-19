#pragma once

// C++ STL
#include <algorithm> // std::find, std::remove
#include <queue>
#include <stdexcept>
#include <string> // std::to_string
#include <vector>

namespace HG::Utils
{
/**
 * @brief Class, controlling unique elements merging.
 * Shall be used in GameObject, Scene and GameObjectController.
 * @todo: Make ContainerType = std::set
 */
template <typename T, typename ContainerType = std::vector<T>>
class DoubleBufferContainer
{
public:
    using container      = ContainerType;
    using const_iterator = typename container::const_iterator;
    using iterator       = typename container::iterator;
    using size_type      = typename container::size_type;

    /**
     * @brief Default constructor creates no elements.
     * @return
     */
    DoubleBufferContainer() : m_removable(), m_added(), m_current()
    {
    }

    /**
     * @brief Copy constructor. It's copying states.
     * @param container Copying object.
     */
    DoubleBufferContainer(const DoubleBufferContainer<T, ContainerType>& container) :
        m_removable(container.m_removable),
        m_added(container.m_added),
        m_current(container.m_current)
    {
    }

    /**
     * @brief Move constructor. It's just moving container data.
     * @param container Movable container.
     */
    DoubleBufferContainer(DoubleBufferContainer<T, ContainerType>&& container) :
        m_removable(std::move(container.m_removable)),
        m_added(std::move(container.m_added)),
        m_current(std::move(container.m_current))
    {
    }

    /**
     * @brief Destructor.
     */
    ~DoubleBufferContainer() = default;

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
        // Dont add to removable if already added
        if ((std::find(m_added.begin(), m_added.end(), e) == m_added.end() ||
             std::find(m_current.begin(), m_current.end(), e) == m_current.end()) &&
            std::find(m_removable.begin(), m_removable.end(), e) != m_removable.end())
        {
            return;
        }

        m_removable.emplace_back(std::move(e));
    }

    /**
     * @brief Method for checking is element removing from
     * container.
     * @param e Element.
     * @return Is it's removing now?
     */
    [[nodiscard]] bool isRemoving(T e) const
    {
        return std::find(m_removable.begin(), m_removable.end(), e) != m_removable.end();
    }

    /**
     * @brief Method to merge all elements. At the beginning it will
     * remove elements from add queue and from current elements container.
     * After that left new elements will be added to current elements container.
     */
    void merge()
    {
        // Removing elements from new one and from current
        typename container::iterator addedNewEndIterator   = m_added.end();
        typename container::iterator currentNewEndIterator = m_current.end();

        for (auto&& el : m_removable)
        {
            addedNewEndIterator   = std::remove(m_added.begin(), addedNewEndIterator, el);
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
     * @brief Method for clearing container.
     */
    void clear()
    {
        m_added.clear();
        m_current.clear();
        m_removable.clear();
    }

    /**
     * @brief Copy operator.
     * @param rhs Right hand container.
     * @return Reference to this container.
     */
    DoubleBufferContainer<T, ContainerType>& operator=(const DoubleBufferContainer<T, ContainerType>& rhs)
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
    DoubleBufferContainer<T, ContainerType>& operator=(DoubleBufferContainer<T, ContainerType>&& rhs)
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
    [[nodiscard]] T& operator[](int i)
    {
#ifndef NDEBUG
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("Size: " + std::to_string(size()) + ", Index: " + std::to_string(i));
        }
#endif

        return m_current[i];
    }

    /**
     * @brief Const index operator.
     * @param i Number of element.
     * @return Copy of element instance.
     */
    [[nodiscard]] T operator[](int i) const
    {
#ifndef NDEBUG
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("Size: " + std::to_string(size()) + ", Index: " + std::to_string(i));
        }
#endif

        return m_current[i];
    }

    [[nodiscard]] size_type size() const
    {
        return m_current.size();
    }

    [[nodiscard]] const container& removable() const
    {
        return m_removable;
    }

    [[nodiscard]] container& removable()
    {
        return m_removable;
    }

    [[nodiscard]] const container& added() const
    {
        return m_added;
    }

    [[nodiscard]] container& added()
    {
        return m_added;
    }

    [[nodiscard]] const container& current() const
    {
        return m_current;
    }

    [[nodiscard]] container& current()
    {
        return m_current;
    }

    iterator begin()
    {
        return m_current.begin();
    }

    const_iterator begin() const
    {
        return m_current.begin();
    }

    const_iterator cbegin() const
    {
        return m_current.cbegin();
    }

    iterator end()
    {
        return m_current.end();
    }

    const_iterator end() const
    {
        return m_current.end();
    }

    const_iterator cend() const
    {
        return m_current.cend();
    }

private:
    container m_removable;
    container m_added;
    container m_current;
};
} // namespace HG::Utils
