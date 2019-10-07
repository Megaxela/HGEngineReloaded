#pragma once

namespace HG::Core
{
class ResourceCache;

/**
 * @brief Class, that describes base for
 * any cachable object.
 */
class CachableObject
{
public:
    /**
     * @brief Constructor.
     * It's pretty hacky, because it uses
     * resource manager's thread_local static
     * variable to get resource cache, that has been
     * used to allocate this cachable object.
     */
    CachableObject();

    /**
     * @brief Virtual destructor for
     * inheritance.
     */
    virtual ~CachableObject() = default;

protected:
    ResourceCache* m_cache;
};
} // namespace HG::Core