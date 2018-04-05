/* My own simple database. It contains information about students.  This base
   supports ordinary operations like searching and printing
*/

#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<exception>
#include<algorithm>

namespace db{

	typedef enum {INT, DOUBLE, STR} Types;

	typedef enum {NONE, TIME} FormatFlags;

	//=======================DATA RECORDS=====================
	struct values {
		int intval;
		double dval;
		std::string strval;
	};

	//contains some field information
	struct FieldInfo {
		std::string name;
		Types type;
		FormatFlags format;
	};

	//contains info about all the fields in the database
	typedef std::vector<FieldInfo> Fields;

	//this class correspondes names of strings to its numbers
	class FieldsID {
		private:
			std::map<std::string, int> correspondence;
		public:
			FieldsID(); 
			void Construct(Fields f);//constructs map by FieldInfo
			int get_ID(const std::string &name);
	};

	//==========================DATA BASE================
	//this class contains information about data records
	typedef std::vector<values> RecordData;

	//list type
	typedef std::list<RecordData> Glist;
	//hash set type
	//struct std::map<values, Glist> HashSet;

	//all database descriptions are at the end
	
	//=======================QUERIES=====================
	//constrains
	typedef enum {EQ, MORE, LESS, LIKE} constrType;
	struct UnitConstraint {
		int constr_on; //field id, which is constrained
		constrType t;
		values v; //needed values
	};
	typedef std::list<UnitConstraint> FilterConstraints;

	//query description
	//DEFECT means that the query wasn't created successfully
	typedef enum {cSELECT, cRESELECT, cPRINT, cINSERT, cREMOVE, cDEFECT} queryType;
	class query {
		private: 
			queryType t;
			FilterConstraints s; //for SELECT and RESELECT queries
			RecordData towrite; //for INSERT queries
			std::vector<int> fields_to_print; 
			
			//these pieces of data are required for PRINT queries
			std::vector<int> fields_to_sort; //fields_to sort
			std::vector<bool> asc_ord; //ascending or descending order
			
			bool check_one_constr(const RecordData &a, const Fields &f, const UnitConstraint &u) const; 
                        //this function checks only one constraint
			
			//these following functions check data of their own types
			bool check_int(int n, int constr_val, constrType ct) const;
			bool check_double(double n, double constr_val, constrType ct) const;
			bool check_string(std::string &n, std::string &constr_val, constrType ct) const;
		public:
			//simple constructor
			//query(queryType _t, FilterConstraints _s, RecordData _towrite);
			//fixed query constructor
			query(queryType t_, FilterConstraints &s_, RecordData &towrite_): 
				t(t_), s(s_), towrite(towrite_) {};
			bool check_record(const RecordData &a, const Fields &f) const; //checking function
			const RecordData& giveDat() const {return towrite;} //it is required for INSERT queries
			queryType giveType() const {return t;} //gives query type
	};
	
	class my_comp {
	private:
	    Fields f; //all the fields
	    std::vector<int> fields_to_compare; //lexigographic order
	    std::vector<bool> asc_ord; //
	    bool compare_unit(const RecordData &a, const RecordData &b, int field_to_c);
	public:
	    my_comp(Fields _f,  std::vector<int> _ftc, std::vector<bool> _asc_ord): 
	      f(_f),  fields_to_compare(_ftc), asc_ord(_asc_ord) {};
	    bool operator()(const RecordData &a, const RecordData &b);
	};
	
	//============RESULT================ 
	class Result {
		private:
			query t; //maybe, query pointer
			int res_code;
			//for SELECT queries we have to retur
			int info; //some quantifical info, i.g. how many cells are selected
			std::list<RecordData> toprint; //result of print queries
			std::list<FieldInfo> fields_to_print; //which fields should we print?  
		public:
			//constructor which doesn't require input lists
		        Result(query _t, int _success, int _info):
				t(_t), res_code(_success), info(_info) {};
			Result(query _t, int _success, int _info, std::list<RecordData> _toprint, 
				std::list<FieldInfo> _ftp):
				t(_t), res_code(_success), info(_info), toprint(_toprint), fields_to_print(_ftp) {};

	};

	//======================SESSION===================
	//user session
	class session {
		friend class Database;
		private:
		  int userID; //userID
		  Glist selected; //what was selected
		public:
		session(int _userID); 
		int get_ID(); 
	};
	
	
	
	//=======================DATABASE===============
	class Database {
		private:
			Fields f;
			FieldsID fid;
			int fields_amount;
			Glist alldat;
			const int hasher; //id of the hasher field

			//these fnctions comfirm operations
			Result SELECT(const query &q, session &u);
			Result RESELECT(const query &q, session &u);
			Result DELETE(const query &q, session &u);
			Result PRINT(const query &q, session &u);
			Result INSERT(const query &q, session &u);
			query Syntax_analyzer(std::string &writed);
		public:
			Database(std::string in); //loads data base from file
			void save(std::string out); //saves the database to a file
			//void load(std::string in); //loads the database
			Result makeit(std::string &writed, session &u); //this functions performs commands
	};

	
	//results codes
	//some errors
	namespace err{
		constexpr int success = 0;
		constexpr int defected_query = -10;
		constexpr int critical = -1;
	}
}
