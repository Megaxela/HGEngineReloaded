#include <gtest/gtest.h>
#include <Transform.hpp>

TEST(Core, TransformDefaultConstructor)
{
    HG::Core::Transform transform;

    ASSERT_EQ(transform.localPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
    ASSERT_EQ(transform.localScale(),    glm::vec3(1.0f, 1.0f, 1.0f));
    ASSERT_EQ(transform.localRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    ASSERT_EQ(transform.globalPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
    ASSERT_EQ(transform.globalScale(),    glm::vec3(1.0f, 1.0f, 1.0f));
    ASSERT_EQ(transform.globalRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    ASSERT_EQ(transform.parent(),     nullptr);
    ASSERT_EQ(transform.gameObject(), nullptr);

    ASSERT_EQ(transform.children().size(), 0);
}

TEST(Core, TransformCopyNoParent)
{
    HG::Core::Transform transform;
    
    transform.setLocalPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    transform.setLocalScale(glm::vec3(4.0f, 5.0f, 6.0f));
    transform.setLocalRotation(glm::quat(7.0f, 8.0f, 9.0f, 10.0f));
    
    HG::Core::Transform copy = transform;
    
    ASSERT_EQ(copy.localPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    ASSERT_EQ(copy.localScale(),    glm::vec3(4.0f, 5.0f, 6.0f));
    ASSERT_EQ(copy.localRotation(), glm::quat(7.0f, 8.0f, 9.0f, 10.0f));
}