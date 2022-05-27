#include <bits/stdc++.h>
using namespace std;

fstream schemafile;

bool checkTable(string tableName){
    schemafile.open("Schema.txt", ios::in);
    string line;
    bool flag=false;
    while(getline(schemafile,line)){
        if(line.substr(0, line.find(' '))==tableName){
            flag=true;
            break;
        }
    }
    schemafile.close();
    return flag;
}

void createTable(vector<string> query){
	fstream table;

    string table_name = query[2];

    if(checkTable(table_name)){
        cout<<"Table Already Exists"<<endl;
        return;
    }

    schemafile.open("Schema.txt", ios::app);
    schemafile << table_name;
    table.open(table_name+".txt", ios::app);


    int start = -1, end = -1;
    for (int i = 3; i < query.size(); i++)
    {
        if (query[i] == "(")
        {
            start = i;
        }
        if (query[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Error" << endl;
        return;
    }

    while (start < end - 1)
    {
        start++;
        if (query[start] != ",")
        {
            schemafile << "  " << query[start];
            //table<<query[start]<<"  " ;
        }
    }
    schemafile << endl;
    //table << endl;
    cout << "Table created successfully" << endl;

    schemafile.close();
}

void displaySchema(){
	schemafile.open("Schema.txt", ios::in);
	string line;
	while (getline(schemafile, line))
    {
        cout<<line<<endl;
    }
    schemafile.close();
}

void dropTable(vector<string> query){

    string table_name = query[2];
    if(!checkTable(table_name)){
        cout<<"Table Not Exists"<<endl;
        return;
    }

    schemafile.open("Schema.txt", ios::in);

    fstream temp;
    temp.open("temp.txt", ios::out);

    string line;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find(' '));
        if (table_name != l1)
        {
            temp << line << endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");

    string f = table_name + ".txt";
    char fileName[f.length()];

    for(int i=0;i<f.length();i++){
        fileName[i] = f[i];
    }
    fileName[f.length()]='\0';
    // cout<<fileName<<endl;
    remove(fileName);

    cout << "Table dropped successfully" << endl;
}

void fetchSchema(string tableName, vector<string> &schema){
    schemafile.open("Schema.txt", ios::in);
    string line;
    int flag = 0;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find(' '));
        if (tableName == l1){
            flag = 1;
            stringstream ss(line);

            while (ss.good()){
                string substr;
                getline(ss, substr, ' ');
                schema.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0){
        cout << "table not found" << endl;
    }

    schemafile.close();
}

bool isNumber(const string& str){
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void getDatatype(string tableName,vector<string> &datatype){
    vector<string> schema;
    fetchSchema(tableName,schema);
    for(int j=4; j<schema.size(); j+=4){
        datatype.push_back(schema[j]);
    }
}

void insert(vector<string> query){
    string table_name = query[2];
    if(!checkTable(table_name)){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    fstream table;
    table.open(table_name + ".txt", ios::app);

    vector<string> datatype;
    getDatatype(table_name, datatype);
    unordered_map<string,int> attributes_table;
    vector<string> schema;
	vector<string> attributes;
    fetchSchema(table_name,schema);
    for(int i=2;i<schema.size();i+=4){
        attributes.push_back(schema[i]);
    }
    int numberofatt=attributes.size();

    int start = -1, end = -1;
    for (int i = 4; i < query.size(); i++)
    {
        if (query[i] == "(")
        {
            start = i;
        }
        if (query[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Error" << endl;
        return;
    }
    int k=0;
    bool invalidType=false;
    int st=start;
    int n=0;

    while (st < end - 1)
    {
        st++;
        if (query[st] != ",")
        {
            n++;
        }
        if(n>numberofatt){
        	cout<<"number of attributes are less"<<endl;
        	return;
		}
        if (query[st] != ",")
        {
            if( ( datatype[k]=="int" && !isNumber(query[st]) ) ||
                ( datatype[k]!="int" && isNumber(query[st]) )){
                cout<<"Invalid Data Type "<<query[st]<<endl;
                return;
            }
            k++;
        }
    }


    while (start < end - 1)
    {
        start++;
        if (query[start] != ",")
        {

            table << query[start];
            if (start != end - 1)
            {
                table << " ";
            }
            else
            {
                table << "\n";
            }
        }
    }


}

void table_number(map<string, int>& table, vector<string> schema){
    int cnt = 0;
    for(int j=2; j<schema.size(); j+=4){
        table[schema[j]] = cnt;
        cnt++;
    }
}

void checkCommand(map<int, string>& column_change, vector<string> query, vector<string> schema){
    int i;
    map<string, int> table;
    table_number(table, schema);
    for(i=3; i<query.size() && query[i]!="where"; i+=3){
        int col = table[query[i]];
        column_change[col] = query[i+2];
    }
}

void update(vector<string> query){
    if(!checkTable(query[1])){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    
    vector<string> schema;
    fetchSchema(query[1], schema);
    
//    map<int, string> column_change;
//    checkCommand(column_change, query, schema);
//	for (auto i : column_change){
//		cout<<i.first<< " "<<i.second<<endl;
//	}
    //cout<<schema[0];
    //cout<<schema[1];
    //cout<<schema[2];
    //cout<<schema[4];
    //all words of that table
    
    if(!schema.empty()){
    	string table_name = query[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios::in);
	    vector<string>::iterator it;
        it = find(query.begin(), query.end(), "where"); //find if there is where in query
        map<int, string> column_change;
        checkCommand(column_change, query, schema);
		for(auto m : column_change){
			cout<<m.first<<" "<<m.second<<endl;
		}
        string line;
        vector<string> lineVec;
        int i, j=0;
        int flag = 0;
        int count = 0;
        fstream temp;
        bool changed=false;
        temp.open("temp.txt", ios::out);
        if(it == query.end()){
            while(getline(table, line)){
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == ' '){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                string new_line = "";
                map<int, string>::iterator itr;
                for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                    lineVec[itr->first] = itr->second;
                }

                for(i=0; i<lineVec.size()-1; i++){
                    new_line += lineVec[i];
                    new_line += " ";
                }
                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }
        else{
            while (getline(table, line)) {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == ' '){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                int idx = 0;
                idx = it - query.begin();
                j=0;
                for(i=2; i<schema.size(); i+=4){
                    if(query[idx+1] == schema[i]){
                        //Equality condition
                        if(query[idx+2] == "="){
                            if(lineVec[j] == query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than condition
                        if(query[idx+2] == ">"){
                            if(lineVec[j] > query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than or equal to
                        if(query[idx+2] == ">="){
                            if(lineVec[j] >= query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than condition
                        if(query[idx+2] == "<"){
                            if(lineVec[j] < query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than and equal to
                        if(query[idx+2] == "<="){
                            if(lineVec[j] <= query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Not equal to condition
                        if(query[idx+2] == "!="){
                            if(lineVec[j] != query[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                //Not making any changes in the row
                if(flag != 1){
                    temp << line << endl;
                    lineVec.clear();
                }
                //make changes in a row
                else{
                    changed=true;
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                        lineVec[itr->first] = itr->second;
                    }

                    for(i=0; i<lineVec.size()-1; i++){
                        new_line += lineVec[i];
                        new_line += " ";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }
                flag = 0;
            }
        }
     	table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        if(changed){
            cout<<"Rows updated successfully"<<endl;
        }
        else{
            cout<<"Value not Present"<<endl;
        }
	}
}

void deleteRow(vector<string> query){
	vector<string> schema;
	if(!checkTable(query[2])){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    fetchSchema(query[2], schema);
    int count = 0;
    if (!schema.empty()){
    	if (query.size() == 3){
    		string table = query[2] + ".txt";
            char c[table.size() + 1];
            strcpy(c, table.c_str());
            remove(c);
            cout << " Table deleted" << endl;
		}
		else if (query[3] == "where"){
			string table_name = query[2];
            fstream table;
            table.open(table_name + ".txt", ios::in);
            string line;

            int i, j;
            j = 0;
            int flag = 0;
            vector<string> lineVec;

            fstream temp;
            temp.open("temp.txt", ios::out);
            
            while (getline(table, line)){
            	stringstream ss(line);
                while (ss.good()){
                	string substr;
                    getline(ss, substr, ' ');
                    lineVec.push_back(substr);
				}
				for (i = 2; i < schema.size(); i += 4){
					if (query[4] == schema[i]){
						if (query[5] == "=")
                        {
                            if (query[6] == lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (query[5] == ">")
                        {
                            if (lineVec[j] < query[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (query[5] == "<")
                        {
                            if (lineVec[j] < query[6] )
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (query[5] == ">=")
                        {
                            if ( lineVec[j] >= query[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (query[5] == "<=")
                        {
                            if (lineVec[j] <= query[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (query[5] == "!=")
                        {
                            if (query[6] != lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
					}
					j++;
				}
				if (flag != 1)
                {
                    temp << line << endl;
                }
                flag = 0;
			}
			table.close();
            temp.close();
            string table1 = table_name + ".txt";
            char c[table1.size() + 1];
            strcpy(c, table1.c_str());
            remove(c);
            rename("temp.txt", c);
            cout << " rows deleted" << endl;
		}
	}
}

void fetchTable(string& tableName, vector<string>& attributes, unordered_map<string,int>& mp,
    vector<vector<string>>& d){
    fstream table(tableName+".txt",ios::in);
    string line;

    cout<<"\n";
    for(auto x:attributes) cout<<x<<"\t";
    cout<<"\n\n";

    while(getline(table,line)){
        stringstream ss(line);
        vector<string> data;

        while (ss.good()){
            string substr;
            getline(ss, substr, ' ');
            data.push_back(substr);
        }

        d.push_back(data);

    }

}

void select(vector<string> query){
	vector<string> attributes;
	auto it = find(query.begin(), query.end(),"from");
	int index = 0;
	if(it!=query.end()){
		index=it-query.begin();
	}
	else{
		cout<<"syntax error"<<endl;
	}
	string tablename=query[index+1];
	
	if(!checkTable(tablename)){
        cout<<"Table Not Exists"<<endl;
        return;
    }
    
    unordered_map<string,int> attributes_table;
    vector<string> schema;

    fetchSchema(tablename,schema);
    
    if(query[1]=="*"){
    	for(int i=2;i<schema.size();i+=4){
        	attributes.push_back(schema[i]);
    	}
	}
	else{
		for(int i=1;i<index;i+=2){
        	attributes.push_back(query[i]);
    	}
	}
    
    int k=0;
    for(int i=2;i<schema.size();i+=4){
        attributes_table[schema[i]]=k++;
    }
    for(auto x: attributes){
    	if(attributes_table.find(x)==attributes_table.end()){
    		cout<<"attributes error"<<endl;
    		break;
		}
	}
	vector<vector<string>> data;
    fetchTable(tablename,attributes,attributes_table,data);
    
    auto it1 = find(query.begin(), query.end(),"where");
    int index1 = 0;
    vector<string> datatype;
    getDatatype(tablename, datatype);
    if(it1!=query.end()){
    	index1=it1-query.begin();
    	string att_name=query[index1+1];
        string att_sign=query[index1+2];
        string att_value=query[index1+3];
        
        int attr_index=attributes_table[att_name];
        if( ( datatype[attr_index]=="int" && !isNumber(att_value) ) ){
                cout<<"Invalid Data Type "<<att_value<<endl;
                return;
            }
            else{
            	for(int k=0;k<data.size();k++){
            if((att_sign=="=" && data[k][attr_index]==att_value)
            || (att_sign==">" && data[k][attr_index]>att_value)
            || (att_sign=="<" && data[k][attr_index]<att_value)
            || (att_sign==">=" && data[k][attr_index]>=att_value)
            || (att_sign=="<=" && data[k][attr_index]<=att_value)
            || (att_sign=="!=" && data[k][attr_index]!=att_value)){
                for(int i=0;i<attributes.size();i++){
                    cout<<data[k][attributes_table[attributes[i]]]<<"\t";
                }
                cout<<endl;
            }
        }
			}

	}
	else{
		for(int k=0;k<data.size();k++){
            for(int i=0;i<attributes.size();i++){
                cout<<data[k][attributes_table[attributes[i]]]<<"\t";
            }
            cout<<endl;
        }
	}
}

void handleQueries(vector<string> query){
    if (query[0] == "create" && query[1] == "table")
    {
        createTable(query);
    }
    else if (query[0] == "drop" && query[1] == "table")
    {
        dropTable(query);
    }
    else if (query[0] == "insert" && query[1] == "into")
    {
        insert(query);
    } 
	else if (query[0] == "update" && query[2] == "set")
    {
        update(query);
    }
    else if (query[0] == "delete" && query[1] == "from")
    {
        deleteRow(query);
    }
	else if (query[0] == "select")
    {
        select(query);
    } 
	else if (query[0] == "display" && query[1] == "all"){
		displaySchema();
	}   
    else
    {
        cout << "Syntax Errors";
    }
}

void extractWords(string input, vector<string> &v){
    string word = "";
    for (auto x : input)
    {
        if (x == ' ')
        {
            v.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    v.push_back(word);
}

int main(){
    vector<string> query;
    string input;

    cout << "enter query>";

    getline(cin, input);

    while (input != "q")
    {

        extractWords(input, query);
        handleQueries(query);

        query.clear();
        cout << "\nenter query>";

        getline(cin, input);


    }

    return 0;
}
