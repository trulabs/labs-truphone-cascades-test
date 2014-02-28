/**
 * Copyright 2014 Truphone
 */
#ifndef SYSTEMPROMPTFACADE_H_
#define SYSTEMPROMPTFACADE_H_

#include <bb/system/SystemPrompt>

namespace truphone
{
namespace test
{
namespace cascades
{
    class SystemPromptFacade : public bb::system::SystemPrompt
    {
    public:
        SystemPromptFacade(const QString& text, QObject * const parent = 0)
            : SystemPrompt(parent),
              returnable(text)
        {
        }

        Q_INVOKABLE QString inputFieldTextEntry() const
        {
            return returnable;
        }
    private:
        const QString returnable;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // SYSTEMPROMPTFACADE_H_
