#include"databasestudents.h"
//===============================db_operations===============================

//TODO indicies
db::Result db::Database::SELECT(const db::query &q, db::session &u)
{
    u.selected.clear();
    //this loop runs through all values in hash set
    for(db::Glist::const_iterator i = alldat.begin(); i != alldat.end(); i++)
    {
            if(q.check_record(*i, f))
            {
                    u.selected.push_back(*i);
            }
    }
    Result res(q, err::success, u.selected.size());
    return res;
}

db::Result db::Database::RESELECT(const db::query &q, db::session &u)
{
    db::Glist temp = u.selected;
    u.selected.clear();
    for(db::Glist::const_iterator i = temp.begin(); i != temp.end(); i++)
    {
            if(q.check_record(*i, f))
            {
                    u.selected.push_back(*i);
            }
    }
    Result res(q, err::success, u.selected.size());
    return res;
}

db::Result db::Database::INSERT(const query &q, session &u)
{
    alldat.push_back(q.giveDat());
    Result res(q, err::success, 1);
    return res;
}

db::Result db::Database::DELETE(const query &q, session &u)
{
    //info about what to delete is in the list u.selected
    size_t before, after;
    before = alldat.size();
    for(db::Glist::const_iterator i = u.selected.begin(); i != u.selected.end(); i++)  {
        all_dat.remove(*i);
    }
    u.selected.clear();
    after = all_dat.size();
    Result res(q, err::success, before - after);
    return res;
}

//TODO
db::Result db::PRINT(const query &q, session &u)
{
    //info about what to print is in the list u.selected
    //first of all, we need to sort the list
    if(q.fields_to_sort.size()) {
        my_comp mc(&f, q.fields_to_sort, q.asc_ord);
        u.selected.sort(mc);
    }

    for(db::Glist::const_iterator i = u.selected.begin(); i != u.selected.end(); i++) {
        for(int j = 0; j < fields_amount; j++) {
            switch (f[j].type):
            {
                case INT:
                    std::cout << (*i)[j].intval;
                    break;
                case DOUBLE:
                    std::cout << (*i)[j].dval;
                    break;
                case STR:
                    std::cout << (*i)[j].strval;
                    break;
                default:
                    std::bad_cast ups;
                    throw(ups);
                    break;
             }


        }
    }
    Result res(q, err::success, 0);
    return res;

}

db::Result db::makeit(std::string &writed, session &u) {
    query q = SyntaxAnalyzer(writed);

    switch (q.t) //queryType
    {
        case cSELECT:
            return SELECT(q, u);
            break;
        case cPRINT:
            return PRINT(q, u);
        case cRESELECT:
            return RESELECT(q, u);
        case cINSERT:
            return INSERT(q, u);
            break;
        case cREMOVE:
            return REMOVE(q, u);
            break;
        case cDEFECT:
            Result r(q, 0, 0);
            return r;
        default:
            std::domain_error ups;
            throw ups;
    }
    Result r(q, 0, 0);
    return r;
}

//=========================================my_comp unit==================================================

bool db::my_comp::compare_unit(RecordData &a, RecordData &b, int field_to_c) {
    int k = field_to_c;
    switch (f[k].type)
    {
        case INT:
            return (a[k].intval < b[k].intval);
            break;
        case DOUBLE:
            return (a[k].dval < b[k].dval);
            break;
        case STR:
            (a[k].strval < b[k].strval);
            break;
     }
    //exceptions
    std::bad_cast ups;
    throw ups;
    return false;

}

bool db::my_comp::operator()(RecordData &a, RecordData &b) {
    //lexicogrraphic comparison
    std::vector<bool>::const_iterator j = asc_ord.begin();
    for(std::vector<int>::const_iterator i = fields_to_compare.begin();
        i != fields_to_compare.end(); i++) {

        //compare two values
        bool res;
        if(*j) {
            res = compare_unit(a, b, *i); //ascending order
        }
        else {
            res = compare_unit(b, a, *i); //descending order
        }
        if(res) return true; //we have found diffrerence
        //another case: values are equal
        bool res1;
        if(*j) {
            res1 = compare_unit(b, a, *i); //ascending order
        }
        else {
            res1 = compare_unit(a, b, *i); //descending order
        }
        if(res1) return false;
    }
    //a and b turned out to be equal
    return false;
}

//==============================================queries and constraints==========================
bool db::query::check_record(const RecordData& a, const Fields& f) const {
   for(Filter_constraints::const_iterator i = s.begin(); i != s.end(); i++) {
       if(!check_one_constr(a, f, *i)) return false;
   }
   return true;

}

bool db::query::check_one_constr(const RecordData& a, const Fields& f, const UnitConstraint& u) const {
    switch (f[u.constr_on].type) {
    case INT:
        return check_int(a[u.constr_on].intval, u.v.intval, u.t);
        break;
    case DOUBLE:
        return check_double(a[u.constr_on].dval, u.v.dval, u.t);
        break;
    case STR:
        return check_string(a[u.constr_on].strval, u.v.strval, u.t);
        break;
    default:
            std::domain_error ups;
            throw ups;
        break;
    }
    return false;
}

bool db::query::check_int(int n, int constr_val, constrType ct) const {
    switch (ct) {
    case EQ:
        return (n == constr_val);
    case MORE:
        return (n > constr_val);
    case LESS:
        return (n < constr_val);
    default:
        std::invalid_argument ups;
        throw ups;
    }
    return false;
}
bool db::query::check_double(double n, double constr_val, constrType ct) const {
    switch (ct) {
    case EQ:
        return (n >= constr_val) && (n <= constr_val);
    case MORE:
        return (n > constr_val);
    case LESS:
        return (n < constr_val);
    default:
        std::invalid_argument ups;
        throw ups;
    }
    return false;
}
bool db::query::check_string(const std::string &n, const std::string &constr_val, constrType ct)
const
{
    switch (ct) {
    case EQ:
        return (n == constr_val);
    case MORE:
        return (n > constr_val);
    case LESS:
        return (n < constr_val);
    case LIKE:
        //the string has the same beginning as given in constr_val
        return (n.find(constr_val) == 0); //TODO: check it
    default:
        std::invalid_argument ups;
        throw ups;
    }
    return false;
}
