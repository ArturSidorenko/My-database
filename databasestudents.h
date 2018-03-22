/* My own simple database. It contains information about students.  This base
   supports ordinary operations like searching and printing
*/

#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<exception>

namespace db{

	typedef enum {INT, DOUBLE, STR} Types;

	typedef enum {NONE, TIME} FormatFlags;

	//=======================DATA RECORDS=====================
	typedef union {
		int intval;
		double dval;
		std::string strval;
	} values;

	//contains some field information
	struct FieldInfo {
		std::string name;
		Types type;
		FormatFlags format;
	};

	//contains info about all the fields in the database
	class Fields {
		std::vector<FieldInfo>;
		public:
		Fields(); //constructs needed fields
		const FieldInfo& get_info(int id);
		Types get_type(int id); 
	};

	//this class correspondes names of strings to its numbers
	class FielsID {
		private:
			std::map<std::string, int> correspondence;
		public:
			FieldsID(); 
			Construct(Fields f);//constructs map by FieldInfo
			int get_ID(std::string name);
	};

	//==========================DATA BASE================
	//this class contains information about data records
	typedef std::vector<values> RecordData;

	//list type
	typedef std::list<RecordData> Glist;
	//hash set type
	struct std::map<int, Glist> HashSet;



	//this class contains all the data
	//it is constructed as a hash-set	

	class Database {
		private:
			Fields f;
			FieldsID fid;
			int fields_amount;
			HashSet hs;
			const int hasher; //id of the hasher field

			//these fnctions comfirm operations
			Result SELECT(const query &q, session &u);
			Result RESELECT(const query &q, session &u);
			Result DELETE(const query &q, session &u);
			Result PRINT(session_dat &u);
			Result INSERT(const RecordData& p);
		public:
			Database(std::string in); //loads data base from file
			void save(std::string out); //saves the database to a file
			//void load(std::string in); //loads the database
			Result makeit(const query &q, session &u); //this functions confirms commands
	};

	class Result {
		private:
			queryType t; 
			int success;
			//for SELECT queries we have to retur
			int info; //some quantifical info, i.g. how many cells are selected
			std::list<RecordData&> toprint; //result of print queries
		public:
			Result(queryType _t, int _success, int _info, std::list<RecordData&> _t):
				t(_t), success(_success), info(_info), toprint(_t) {};

	};
	//some errors
	namespace err{
		constexpr int success = 0;
		constexpr int defected_query = -10;
		constexpr int critical = -1;
	}

	//=======================QUERIES=====================
	//constrains
	typedef enum {EQ, MORE, LESS, LIKE} constrType;
	struct UnitConstraint {
		int constr_on; //field id, which is constrained
		constrType t;
		values v, v_upper, v_below; //needed values
	};
	typedef std::list<UnitConstraint> FilterConstraints;

	//query description
	//DEFECT means that the query wasn't created successfully
	typedef enum {SELECT, RESELECT, PRINT, INSERT, REMOVE, DEFECT} queryType;
	class query {
		private: 
			queryType t;
			FilterConstraints s; //for SELECT and RESELECT queries
			RecordData &towrite; //for INSERT queries

			bool check_one_constr(const RecordData &a, const UnitConstraint &u); 
                        //this function checks only one constraint
			//these following functions check data of their own types
			bool check_int(const int n, const UnitConstraint &u);
			bool check_double(const double x, const UnitConstraint &u);
			bool check_string(const std::string &s, const UnitConstraint &u);
		public:
			//simple constructor
			//query(queryType _t, FilterConstraints _s, RecordData _towrite);
			//fixed query constructor
			query(int what_do_you_want);
			bool check_record(const RecordData &a); //checking function
			const RecordData& giveDat(); //it is required for 
	};


	//======================SESSION===================
	//user session
	class session_dat {
		friend class Database;
		private:
		int userID; //userID
		Glist selected; //what was selected
		public:
		session(int _userID); 
		int get_ID(); 
	};
};
