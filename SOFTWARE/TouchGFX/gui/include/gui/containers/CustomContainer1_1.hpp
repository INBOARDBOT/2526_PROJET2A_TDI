#ifndef CUSTOMCONTAINER1_1_HPP
#define CUSTOMCONTAINER1_1_HPP

#include <gui_generated/containers/CustomContainer1_1Base.hpp>


class CustomContainer1_1 : public CustomContainer1_1Base
{
public:
    CustomContainer1_1();
    virtual ~CustomContainer1_1() {}

    virtual void setText(const char* string_text);
    virtual void initialize();
protected:
};

#endif // CUSTOMCONTAINER1_1_HPP
