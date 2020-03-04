
template <class TYPE>
class MyList{
	struct ListItem{
		TYPE key_;
		ListItem *next_;

		ListItem (TYPE key, ListItem *next=NULL){
			key_=key;
			next_=next;
		}
	};
	ListItem *first_;
	
	int count_;
public:
	ListItem *external;
	//MyList(): count_(0), first_(NULL){cout<<"Констр-р создал пустой список\n";}
	MyList (){
		count_=0;
		first_=NULL;
		external=NULL;
		//Debug("Констр-р создал пустой список\n");
	}

	MyList (const MyList & x){
		for (count_=0; first_->next_!=NULL; count_++, first_=first_->next_)	first_=x.first_;
		//Debug("Констр-р копирования\n");
	}

	 void Release (){
		//Debug("Дест-р\n");
		if (!IsEmpty ()){
			ListItem *cur;
			for (cur=first_; cur!=NULL; ){
				cur=cur->next_;
				delete first_;
				first_=cur;
			}
		}
                count_=0;
		first_=NULL;
		external=NULL;
	 }
        
        ~MyList (){
		//Debug("Дест-р\n");
		if (!IsEmpty ()){
			ListItem *cur;
			for (cur=first_; cur!=NULL; ){
				cur=cur->next_;
				delete first_;
				first_=cur;
			}
		}
	 }

	int GetCount (){ return count_; }

	bool IsEmpty () const { return (first_==NULL) ? true: false; }

	ListItem  *GetFirst () const{
		if (!IsEmpty ()) return first_;
		else {
			//Debug("Список пуст\n");
			return NULL;}
	}

	ListItem *GetItem (int id){
		if (id>=0 && id<count_){
			ListItem *cur=first_;
			for (int i=0; i<id; i++) cur=cur->next_;

			return cur;
		}

		return NULL;
	}

	TYPE *GetItemVal (int id){
		if (id>=0 && id<count_){
			ListItem *cur=first_;
			for (int i=0; i<id; i++) cur=cur->next_;

			return &cur->key_;
		}

		return NULL;
	}

	void Add (TYPE newKey);

	void Remove (int id);

	/*
	void printALL () const{
		if (!IsEmpty ()){
			for (ListItem *cur=first_; cur!=NULL; cur=cur->next_) 
				cout<<cur->key_<<' ';
			cout<<'\n';
		}
		//else Debug("Список пуст\n");
	}
	*/
        void AddFirst (TYPE newKey){
		ListItem *newItem =new ListItem (newKey); 
		newItem->next_=first_;
		count_++;
		if (first_==NULL){
			first_=newItem;
		}
		else  first_=newItem;
	}



};

template <class TYPE>
void
MyList<TYPE>::
Add (TYPE newKey){
		ListItem *newItem =new ListItem (newKey); 

		if (!IsEmpty()){
			ListItem *cur=first_;
			while (cur->next_) cur=cur->next_;
			cur->next_=newItem;
		}
		else first_=newItem;

		count_++;
	}

template <class TYPE>
void
MyList<TYPE>::
Remove (int id){
		
		if (id==0) first_=first_->next_;
		else
		{	id=id-1;
			bool fl=false;
			ListItem *cur; int i=0;
			for (ListItem *prev=first_; fl!=true && prev->next_!=NULL ; prev=prev->next_){
				cur=prev->next_;
				if (i==id) {fl=true; prev->next_=cur->next_;}
				i++;
			}
		delete cur;
		}
		count_--;
		
	}
