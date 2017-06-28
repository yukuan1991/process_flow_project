#include <QToolButton>

class ribbon_tool : public QToolButton
{
    Q_OBJECT
public:
    template<typename ... Args>
    ribbon_tool (Args && ... p) : QToolButton (std::forward<Args> (p)...) {}
};

