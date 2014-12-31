//@author Andrzej Siadkowski
//data container with for proteins

#ifndef PROTEINDATA
#define PROTEINDATA
class ProteinData {
public:
	ProteinData ();
	~ProteinData ();


	void setData1 (const float& data);
	void setData2 (const float& data);
	void setData3 (const float& data);
	void setReactionResult (const int & result);
	void setId (const int & i);

	float getData1 ();
	float getData2 ();
	float getData3 ();
	int getReactionResult ();
	int getId ();



private:
	float data1;
	float data2;
	float data3;
	int reactionResult;
	int id;

};

#endif