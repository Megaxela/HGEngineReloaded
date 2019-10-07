#pragma once

// Declarations for simple behaviour coding
// C++ STL
#include <any>
#include <functional>

#define HG_PROPERTY_INITIALIZER_RAW_OBJ(BEHAVIOUR, METHOD_OWNER, NAME, TYPE, SETTER, GETTER)                           \
    HG::Core::PropertyInitializer<TYPE> __##NAME##Init = HG::Core::PropertyInitializer<TYPE>(                          \
        BEHAVIOUR,                                                                                                     \
        #NAME,                                                                                                         \
        #TYPE,                                                                                                         \
        std::bind(&std::remove_reference<decltype(*METHOD_OWNER)>::type::SETTER, METHOD_OWNER, std::placeholders::_1), \
        std::bind(&std::remove_reference<decltype(*METHOD_OWNER)>::type::GETTER, METHOD_OWNER))

#define HG_PROPERTY_INITIALIZER_RAW(NAME, TYPE, SETTER, GETTER) \
    HG_PROPERTY_INITIALIZER_RAW_OBJ(this, this, NAME, TYPE, SETTER, GETTER)

#define HG_PROPERTY_INITIALIZER(TYPE, NAME) \
    HG_PROPERTY_INITIALIZER_RAW(NAME, TYPE, setProperty##NAME, getProperty##NAME)

#define HG_PROPERTY_DEFAULT(TYPE, NAME, DEFAULT_VARIABLE) \
public:                                                   \
    void setProperty##NAME(TYPE value)                    \
    {                                                     \
        m_property##NAME = value;                         \
    }                                                     \
                                                          \
    TYPE getProperty##NAME() const                        \
    {                                                     \
        return m_property##NAME;                          \
    }                                                     \
                                                          \
private:                                                  \
    TYPE m_property##NAME = DEFAULT_VARIABLE;             \
    HG_PROPERTY_INITIALIZER(TYPE, NAME)

#define HG_PROPERTY(TYPE, NAME)        \
public:                                \
    void setProperty##NAME(TYPE value) \
    {                                  \
        m_property##NAME = value;      \
    }                                  \
                                       \
    TYPE getProperty##NAME() const     \
    {                                  \
        return m_property##NAME;       \
    }                                  \
                                       \
private:                               \
    TYPE m_property##NAME;             \
    HG_PROPERTY_INITIALIZER(TYPE, NAME)

namespace HG::Core
{
class GameObject;
class Input;
class Scene;

/**
 * @brief Class, that describes abstract
 * gameobject behaviour component.
 */
class Behaviour
{
public:
    // Property system
    // todo: Low std::any overhead with another api
    struct Property
    {
        Property(std::string name, std::string type, std::any setter, std::any getter, const std::type_info& typeInfo) :
            m_name(std::move(name)),
            m_type(std::move(type)),
            m_setter(std::move(setter)),
            m_getter(std::move(getter)),
            m_typeInfo(typeInfo)
        {
        }

        std::string name() const
        {
            return m_name;
        }

        std::string type() const
        {
            return m_type;
        }

        const std::type_info& typeInfo() const
        {
            return m_typeInfo;
        }

        template <typename Type>
        std::function<Type()> getGetter() const
        {
            return std::any_cast<std::function<Type()>>(m_getter);
        }

        template <typename Type>
        std::function<void(Type)> getSetter() const
        {
            return std::any_cast<std::function<void(Type)>>(m_setter);
        }

        std::string m_name;
        std::string m_type;

        std::any m_setter;
        std::any m_getter;

        const std::type_info& m_typeInfo;
    };

    enum class Type
    {
        Logic,
        Render
    };

    /**
     * @brief Constructor.
     */
    explicit Behaviour(Type t = Type::Logic);

    /**
     * @brief Destructor.
     */
    virtual ~Behaviour();

    /**
     * @brief Method for getting behaviour type.
     * @return Behaviour type.
     */
    Type type() const;

    /**
     * @brief Method for checking is behaviour enabled.
     * If behaviour is disabled, it will not be called in update.
     * @return Is behaviour enabled.
     */
    bool isEnabled() const;

    /**
     * @brief Method for setting behaviour enabled.
     * @param value Enabled value.
     */
    void setEnabled(bool value);

    /**
     * @brief Public method for calling
     * overridden update methods.
     */
    virtual void update();

    /**
     * @brief Public method for calling
     * start method.
     */
    void start();

    /**
     * @brief Method for getting gameobject.
     * @return Pointer to parent gameobject.
     */
    HG::Core::GameObject* gameObject() const;

    /**
     * @brief Method for receiving parent scene.
     * @return Scene.
     */
    HG::Core::Scene* scene() const;

    /**
     * @brief Method for getting input controller
     * from application;
     * @return Constant pointer to input controller.
     */
    const Input* input() const;

    /**
     * @brief Method for adding property to behaviour.
     * @param name Property name.
     * @param property Property object.
     */
    void addProperty(Property property);

    /**
     * @brief Method for setting property value.
     * @tparam ArgumentType Argument type.
     * @param name Property name.
     * @param value Value.
     * @return Setting result.
     */
    template <typename ArgumentType>
    void setProperty(const std::string& name, ArgumentType value)
    {
        auto propertyIter =
            std::find_if(m_properties.begin(),
                         m_properties.end(),
                         [&name](const std::pair<std::string, Property>& pair) { return pair.first == name; });

        if (propertyIter == m_properties.end())
        {
            throw std::invalid_argument("Unknown property \"" + name + "\"");
        }

        propertyIter->second.template getSetter<ArgumentType>()(value);
    }

    /**
     * @brief Method for getting property value.
     * @tparam Type Argument type.
     * @param name Property name.
     */
    template <typename Type>
    Type getProperty(const std::string& name)
    {
        auto propertyIter =
            std::find_if(m_properties.begin(),
                         m_properties.end(),
                         [&name](const std::pair<std::string, Property>& pair) { return pair.first == name; });

        if (propertyIter == m_properties.end())
        {
            throw std::invalid_argument("Unknown property \"" + name + "\"");
        }

        return propertyIter->second.template getGetter<Type>()();
    }

    /**
     * @brief Method for getting all properties.
     * @return Vector with all properties.
     */
    std::vector<Property> getProperties() const;

    /**
     * @brief Method for getting all properties.
     * @param container Vector, that will be filled with properties.
     */
    void getProperties(std::vector<Property>& container) const;

protected:
    /**
     * @brief Virtual method, that's called
     * once on scene starting.
     */
    virtual void onStart();

    /**
     * @brief Virtual method, that's called
     * every frame.
     */
    virtual void onUpdate();

    /**
     * @brief Virtual method, that's called
     * after fixed time. (If base framerate more
     * than fixed update interval)
     */
    virtual void onFixedUpdate();

    friend class GameObject;

    /**
     * @brief Method for setting parent gameobject.
     * @param ptr Parent gameobject.
     */
    void setParentGameObject(HG::Core::GameObject* ptr);

private:
    Type m_type;

    bool m_enabled;

    HG::Core::GameObject* m_parent;

    std::vector<std::pair<std::string, Property>> m_properties;
};

template <typename T>
class PropertyInitializer
{
public:
    PropertyInitializer(HG::Core::Behaviour* b,
                        std::string valName,
                        std::string typeName,
                        std::function<void(T)> setter,
                        std::function<T()> getter)
    {
        b->addProperty(HG::Core::Behaviour::Property(
            std::move(valName), std::move(typeName), std::move(setter), std::move(getter), typeid(T)));
    }
};
} // namespace HG::Core
