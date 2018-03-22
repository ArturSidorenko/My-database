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
        myout << f[i].name << f[i].type << f[i].format;
    }
    
    //writing down the hasher field
    myout << hasher << std::endl;
    
    //writing down main info
    //this loop runs through all values in hash set
    for(HashSet::const_iterator i = hs.begin(); i != hs.end(); i++)
    {
        //this loop runs through list corresponding to the key
        for(Glist::const_iterator j = (*i).begin(); j != (*i).end(); j++)
        {
            //this loop runs through cells in the record
            for(RecordData::const_iterator k = (*j).begin(); k != (*j).end(); k++)
            {
                myout << k;
            }
            myout << std::endl;
                
        }
    }
     
}

void db::Database::load(std::string in)
{
    std::ifstream fs(in);
    csv::csv_istream myin(fs);
    
    //reading total amount of fields
    myin >> fields_amount;
    //readind info about fields
    
}