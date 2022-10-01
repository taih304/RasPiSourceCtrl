

struct link-object {
	link-object* next;
	int val;
}


link-object* head;


add_object_into_linkList(link-object* sth)
{
	link-object* temp = // allocate new link-object
	
	[temp->next = sth->next;
	temp->val = sth->val;]

    or
    
    [memcpy(temp, sth, size...)]


	
	head->next = temp;
	// head->next = sth;

}

int main()
{
	link-object* obj = // allocate new link-object
	add_object_into_linkList(obj);
	
	free(obj);
	
	
	// program continue doing sth
}


[ head ->  obj -> null]