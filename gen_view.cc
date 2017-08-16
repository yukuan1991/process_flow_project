#include "gen_view.h"
#include "gen_model.h"


using namespace std;

gen_view::gen_view (QWidget *parent)
    :QTableView (parent)
    ,model_ (make_unique<gen_model> (this))
{
    setModel (model_.get ());
    model_->set_row (7);
}

gen_view::~gen_view()
{

}
