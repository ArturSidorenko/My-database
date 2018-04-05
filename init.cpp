#include <fstream>
#include <iostream>
#include "text/csv/ostream.hpp"
#include "text/csv/istream.hpp"
#include "databasestudents.h"

namespace csv = ::text::csv;

void db::Database::save(std::string out)
{
    std::ofstream fs(out);
    csv::csv_ostream myout(fs);
    
    //writing down total amount of fields
    myout << fields_amount << std::endl;
    //writing down info about fields
    for(int i = 0; i < fields_amount; i++)
    {
        myout << f[i].name << f[i].type << f[i].format << std::endl;
    }
    
    myout << all_dat.size() << std::endl;
    //writing down main info
    //this loop runs through all values in hash set
    //this loop runs through list corresponding to the key
    for(Glist::const_iterator j = (*i).begin(); j != (*i).end(); j++)
    {
        //this loop runs through cells in the record
        for(int i = 0; i < fields_amount; i++)
        {
            switch(f[i].type) {
            case INT:
                myout << (*j)[i].intval;
                break;
            case STR:
                myout << (*j)[i].strval;
                break;
            case DOUBLE:
                myout << (*j)[i].dval;
                break;

            }
        }
        myout << std::endl;
                
    }
     
}

void db::Database::Database(std::string in)
{
    std::ifstream fs(in);
    csv::csv_istream myin(fs);
    
    //reading total amount of fields
    myin >> fields_amount;
    //readind info about fields
    for(int i = 0; i < fields_amount; i++) {
        FieldInfo fio;
        myin >> fio.name;
        myin >> fio.type;
        myin >> fio.format;
        f[i] = fio;
    }
    //it's time to build FieldsID tree
    fid.Construct(f);
    //reading info about indicies
    //reading info about data
    int base_size;
    myin >> base_size;
    for(int i = 0; i < base_size; i++ ) {
        RecordData rd;
        for(int j = 0; j < fields_amount; j++) {
            switch(f[i].type) {
            case INT:
                myin >> rd[j].intval;
                break;
            case STR:
                myin >> rd[j].strval;
                break;
            case DOUBLE:
                myin >> rd[j].dval;
                break;
            }
        }
        all_dat.push_back(rd);
    }
    
}

//========================FieldsIDs
void db::FieldsID::Construct(Fields f) {
    for(int i = 0; i < f.size(); i++) {
        correspondence.emplace((f[i]).name, i);
    }
}

int db::FieldsID::get_ID(const std::string &name) {
   std::map<std::string, int>::const_iterator a = find(name);
   if(a == correspondence.end()) {
       std::bad_exception ups;
       throw ups;
   }
   return a -> second;
}
