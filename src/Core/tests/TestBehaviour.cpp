#include <gtest/gtest.h>

#include <HG/Core/Behaviour.hpp>

class BlankBehaviour : public HG::Core::Behaviour
{

};

TEST(Core, BehaviourEmpty)
{
    BlankBehaviour blankBehaviour;

    ASSERT_EQ(blankBehaviour.type(),       HG::Core::Behaviour::Type::Logic);
    ASSERT_EQ(blankBehaviour.isEnabled(),  true);
    ASSERT_EQ(blankBehaviour.gameObject(), nullptr);
    ASSERT_EQ(blankBehaviour.scene(),      nullptr);
    ASSERT_EQ(blankBehaviour.input(),      nullptr);

    std::vector<HG::Core::Behaviour::Property> properties1;
    blankBehaviour.getProperties(properties1);

    auto properties2 = blankBehaviour.getProperties();

    ASSERT_EQ(properties1.size(), 0);
}

class PropertyBehaviour : public HG::Core::Behaviour
{
    HG_PROPERTY(uint8_t, UInt8_Property);
    HG_PROPERTY(uint32_t, UInt32_Property);
};

TEST(Core, BehaviourProperties)
{
    PropertyBehaviour propertyBehaviour;

    ASSERT_EQ(propertyBehaviour.type(),       HG::Core::Behaviour::Type::Logic);
    ASSERT_EQ(propertyBehaviour.isEnabled(),  true);
    ASSERT_EQ(propertyBehaviour.gameObject(), nullptr);
    ASSERT_EQ(propertyBehaviour.scene(),      nullptr);
    ASSERT_EQ(propertyBehaviour.input(),      nullptr);

    std::vector<HG::Core::Behaviour::Property> properties1;
    propertyBehaviour.getProperties(properties1);

    auto properties2 = propertyBehaviour.getProperties();

    ASSERT_EQ(properties1.size(), 2);

    ASSERT_EQ(properties1[0].type(), "uint8_t");
    ASSERT_EQ(properties1[0].name(), "UInt8_Property");
    ASSERT_EQ(properties1[0].typeInfo(), typeid(uint8_t));
    properties1[0].getSetter<uint8_t>()(12);
    ASSERT_EQ(properties1[0].getGetter<uint8_t>()(), 12);

    ASSERT_EQ(properties1[1].type(), "uint32_t");
    ASSERT_EQ(properties1[1].name(), "UInt32_Property");
    ASSERT_EQ(properties1[1].typeInfo(), typeid(uint32_t));
    properties1[1].getSetter<uint32_t>()(32);
    ASSERT_EQ(properties1[1].getGetter<uint32_t>()(), 32);
}