db::Result db::Database::SELECT(const db::query &q, db::session &u)
{
    u.selected.clear();
    //this loop runs through all values in hash set
    for(db::HashSet::const_iterator i = hs.begin(); i != hs.end(); i++)
    {
        //this loop runs through list corresponding to the key
        for(db::Glist::const_iterator j = (*i).begin(); j != (*i).end(); j++)
        {
            if(q.check_record(*j))
            {
                u.selected.push_back();
            }
        }
    }
}

db::Result db::Database::RESELECT(const db::query &q, db::session &u)
{
    db::Glist temp = u.selected;
    u.selected.clear();
    for(db::Glist::const_iterator i = temp.begin(); i != temp.end(); i++)
    {
            if(q.check_record(*j))
            {
                    u.selected.push_back();
            }
    }
}

db::Result db::Database::INSERT(const db::RecordData& p)
{
    db::Glist::iterator needed = HashSet.find(p[haser]);
    if(needed == HashSet.end()) 
    {
        //new list creating
    }
    
}