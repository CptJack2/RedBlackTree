#pragma once
template<class K,class V>
class RBTreeNode{
public:
	K key;
	RBTreeNode* left;
	RBTreeNode* right;
	RBTreeNode* parent;
	V value;
	char color;
	RBTreeNode(K k, V v):key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), color('r') {}
	RBTreeNode(K k, V v, RBTreeNode* p) :key(k), value(v), left(nullptr), right(nullptr), parent(p), color('r') {}
	void build_left(K k, V v) {left=new RBTreeNode<K,V>(k, v,this);}
	void build_right(K k, V v) { right = new RBTreeNode<K, V>(k, v, this); }
};
template<class K,class V>
class RBTreeMap {
	RBTreeNode<K,V>* root;
	int _size;
	void insert_fixup(RBTreeNode<K, V>*);
	void left_rotate(RBTreeNode<K, V>*);
	void right_rotate(RBTreeNode<K, V>*);
	friend void test(RBTreeMap& m);
	RBTreeNode<K, V>* get_node_ptr(const K& k);
	void remove_fixup(RBTreeNode<K, V>*);
public:
	RBTreeMap():_size(0),root(nullptr){}
	void insert(const K& k,const V& v);
	void remove(const K&);
	bool has_key(const K& k);
	int size() { return _size; }
};
template<class K, class V>
void RBTreeMap<K, V>::remove_fixup(RBTreeNode<K, V>* x) {
	while (x != root && x->color == 'b') {//xΪ��+�ڽڵ��Ҳ�Ϊroot��Ϊrootֱ������rootΪ�ڣ�Ϊ��+��ֱ����Ϊ�켴�ɣ�
		if (x == x->parent->left) {//��x������
			RBTreeNode<K, V>* w = x->parent->right;//w����x���ֵ�
			//case1��xΪ�ڣ�wΪ�죬pxΪ�ڣ�wl/wrΪ��
			//�������Ϊ��+�ڣ��츸�����ֵܵ�case2/3/4
			if (w->color == 'r') {
				w->color = 'b';//���ֵ���Ϊ��
				x->parent->color = 'r';//��������Ϊ��
				left_rotate(x->parent);//�Ը�������
				w = x->parent->right;//��������x���ֵ�
			}
			//case2��xΪ��+�ڣ�wΪ�ڣ�wl/wr��Ϊ��
			//��������Ҳ����-1������Ϊx�ĸ��ڵ�+��
			if ((!w->left|| w->left->color == 'b' )&&( !w->right|| w->right->color == 'b')) {
				w->color = 'r';//�ֵ���Ϊ��
				x = x->parent;//xתΪ�丸�ڵ�
			}
			else{ 
				//case3,x��+�ڣ�w�ڣ�wl�죬wr��
				//������󣬱�Ϊ��+�ڣ����ֵܣ��ֵ�����Ϊ���case4
				if (!w->right||w->right->color == 'b') {
					w->left->color = 'b';//wl���
					w->color = 'r';//w���
					right_rotate(w);//w����
					w = x->parent->right;//����x���ֵ�
				}
				//case4����+�ڣ����ֵܣ��ֵ�����Ϊ�죬����������ɫ��case4
				//�������x��������ĺ�ɫ��������ƽ��
				w->color = x->parent->color;//��px��ɫ����w
				x->parent->color = 'b';//px��Ϊ��
				w->right->color = 'b';//wr��Ϊ��
				left_rotate(x->parent);//����px
				x = root;//�������
			}
		}
		else {
			if (x == x->parent->right) {//��x���Һ���
				RBTreeNode<K, V>* w = x->parent->left;//w����x���ֵ�
				//case1��xΪ�ڣ�wΪ�죬pxΪ�ڣ�wl/wrΪ��
				//�������Ϊ��+�ڣ��츸�����ֵܵ�case2/3/4
				if (w->color == 'r') {
					w->color = 'b';//���ֵ���Ϊ��
					x->parent->color = 'r';//��������Ϊ��
					right_rotate(x->parent);//�Ը�������
					w = x->parent->left;//��������x���ֵ�
				}
				//case2��xΪ��+�ڣ�wΪ�ڣ�wl/wr��Ϊ��
				//��������Ҳ����-1������Ϊx�ĸ��ڵ�+��
				if ((!w->left || w->left->color == 'b') && (!w->right || w->right->color == 'b')) {
					w->color = 'r';//�ֵ���Ϊ��
					x = x->parent;//xתΪ�丸�ڵ�
				}
				else {
					//case3,x��+�ڣ�w�ڣ�wl�죬wr��
					//������󣬱�Ϊ��+�ڣ����ֵܣ��ֵ�����Ϊ���case4
					if (!w->left||w->left->color == 'b') {
						w->right->color = 'b';//wl���
						w->color = 'r';//w���
						left_rotate(w);//w����
						w = x->parent->left;//����x���ֵ�
					}
					//case4����+�ڣ����ֵܣ��ֵ�����Ϊ�죬����������ɫ��case4
					//�������x��������ĺ�ɫ��������ƽ��
					w->color = x->parent->color;//��px��ɫ����w
					x->parent->color = 'b';//px��Ϊ��
					w->left->color = 'b';//wl��Ϊ��
					right_rotate(x->parent);//����px
					x = root;//�������
				}			}		}	}
	x->color = 'b';//����+��ڵ�ֱ������Ϊ�켴��
}
template<class K, class V>
void RBTreeMap<K, V>::remove(const K& k) {
	RBTreeNode<K, V>* z = get_node_ptr(k),*y,*x;//z����洢Ҫɾ����key�Ľڵ㣬y����ʵ�ʱ�ɾ���Ľڵ㣨��y��kv�ƶ���z��
	if (!z->left || !z->right)//���zֻ��һ����������ΪҶ��
		y = z;
	else {//���z�����������������ĺ�̣�������������ڵ㣩
		y = z->right;
		while (y->left)
			y = y->left;
		z->key = y->key;
		z->value = y->value;
	}
	//��ʱ��yֻ����0/1����������x����y������
	if (y->left)
		x = y->left;
	else
		x = y->right;
	//�����ɾ�ڵ���Ҷ��,�Ҳ�Ϊ����һ�������������Ϊһ����NULL�ڵ�
	if (!y->left && !y->right && y!=root) {
		if(y->color=='b')
			remove_fixup(y);
		//������ɾ��y
		if (y == y->parent->left)
			y->parent->left = nullptr;
		else
			y->parent->right = nullptr;
		--_size;
		delete y;
	}
	else {//��ɾ���Ľڵ�ֻ��һ������
		//������ɾ��y
		if(x)
			x->parent = y->parent;
		if (!y->parent)//����ɾ�����Ǹ��ڵ㣬��Ҫ����root
			root = x;
		else if (y == y->parent->left)
			y->parent -> left = x;
		else
			y->parent->right = x;
		--_size;
		if (root&& y->color == 'b')//�����ɾ���ڵ�Ϊ��ɫ�����������Ϊ��ɫֱ��ɾ������
				remove_fixup(x);
		delete y;
	}
}
template<class K, class V>
bool RBTreeMap<K, V>::has_key(const K& k) {
	if (get_node_ptr(k))
		return true;
	else
		return false;
}
template<class K, class V>
RBTreeNode<K, V>* RBTreeMap<K, V>::get_node_ptr(const K& k) {
	RBTreeNode<K, V>* p = root;
	while (p && p->key != k) {
		if (k > p->key)
			p = p->right;
		else if (k < p->key)
			p = p->left;
	}
	return p;
}
template<class K, class V>
void RBTreeMap<K, V>::insert(const K& k, const V& v) {
	//������δ����
	if (_size == 0) {
		root = new RBTreeNode<K, V>(k, v);
		root->color = 'b';
		++_size;
		return;
	}
	//�ҵ���Ӧ�����Ҷ�ӽڵ�
	RBTreeNode<K, V>* p = root,*t;
	while (1) {
		if (k < p->key)
			if (p->left)
				p = p->left;
			else{
				p->build_left(k, v);
				t = p->left;
				break;
			}
		else if (k>p->key)
			if (p->right)
				p = p->right;
			else {
				p->build_right(k, v);
				t = p->right;
				break;
			}
		else//��ͬkeyֵ����������
			return;
	}
	//��Ҷ�ӽڵ����
	//RBTreeNode<K, V>* t;
	//if (k < p->key){
	//	p->build_left(k, v);
	//	t = p->left;}
	//else if (k>p->key){
	//	p->build_right(k,v);
	//	t = p->right;}
	//else//��ͬkeyֵ����������
	//	return;
	++_size;
	insert_fixup(t);
}
template<class K, class V>
void RBTreeMap<K, V>::insert_fixup(RBTreeNode<K, V>* z) {
	while (z->parent && z->parent->color == 'r') {// ������ǰ�ڵ�(z)�ĸ��ڵ��Ǻ�ɫ������������´���(��������ڵ㣬ֱ���˳����ɣ�
		if (z->parent == z->parent->parent->left) {// ����z�ĸ��ڵ㡱�ǡ�z���游�ڵ�����ӡ�����������´���
			RBTreeNode<K, V>* y = z->parent->parent->right;// ��y����Ϊ��z������ڵ�(z���游�ڵ���Һ���)��
			if (y&& y->color == 'r') {// Case 1�����������Ǻ�ɫ
				z->parent->color = 'b';//  (01) �������ڵ㡱��Ϊ��ɫ
				y->color = 'b';//  (02) ��������ڵ㡱��Ϊ��ɫ��
				z->parent->parent->color='r';//  (03) �����游�ڵ㡱��Ϊ����ɫ����
				z = z->parent->parent;//  (04) �����游�ڵ㡱��Ϊ����ǰ�ڵ㡱(��ɫ�ڵ�)
			}
			else{//�����岻���ڣ����Ϊ����Ϊ�ڽڵ�����
				//����->�ҵ������Ϊ��->��
				if (z == z->parent->right) {// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
					z = z->parent; //  (01) �������ڵ㡱��Ϊ���µĵ�ǰ�ڵ㡱��
					left_rotate(z);//  (02) �ԡ��µĵ�ǰ�ڵ㡱Ϊ֧�����������
				}
				//������->������
				z->parent->color = 'b';// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�(01) �������ڵ㡱��Ϊ����ɫ����
				z->parent->parent->color = 'r';//  (02) �����游�ڵ㡱��Ϊ����ɫ����
				right_rotate(z->parent->parent); //  (03) �ԡ��游�ڵ㡱Ϊ֧�����������
			}
		}
		else {// ����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�����������´���
			RBTreeNode<K, V>* y = z->parent->parent->left;// ��y����Ϊ��z������ڵ�(z���游�ڵ������)��
			if (y&& y->color == 'r') {// Case 1�����������Ǻ�ɫ
				z->parent->color = 'b';//  (01) �������ڵ㡱��Ϊ��ɫ
				y->color = 'b';//  (02) ��������ڵ㡱��Ϊ��ɫ��
				z->parent->parent->color = 'r';//  (03) �����游�ڵ㡱��Ϊ����ɫ����
				z = z->parent->parent;//  (04) �����游�ڵ㡱��Ϊ����ǰ�ڵ㡱(��ɫ�ڵ�)
			}
			else {//�����岻���ڣ����Ϊ����Ϊ�ڽڵ�����
				//����->��������Ϊ��->��
				if (z == z->parent->left) {// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
					z = z->parent; //  (01) �������ڵ㡱��Ϊ���µĵ�ǰ�ڵ㡱��
					right_rotate(z);//  (02) �ԡ��µĵ�ǰ�ڵ㡱Ϊ֧�����������
				}
				//������->�ҵ����
				z->parent->color = 'b';// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�(01) �������ڵ㡱��Ϊ����ɫ����
				z->parent->parent->color = 'r';//  (02) �����游�ڵ㡱��Ϊ����ɫ����
				left_rotate(z->parent->parent); //  (03) �ԡ��游�ڵ㡱Ϊ֧�����������
			}
		}
	}
	root->color = 'b';//���ڵ�ȾΪ��ɫ
}
template<class K, class V>
void RBTreeMap<K, V>::left_rotate(RBTreeNode<K, V>* t) {
	//��t��parent��Ϊt->right��parent
	RBTreeNode<K, V>* tmp = t->parent;
	if (t->parent) {
		if (t == t->parent->left) {
			t->parent->left = t->right;
		}
		else {
			t->parent->right = t->right;
		}
	}
	t->parent = t->right;
	t->right->parent = tmp;
	//��t->right����������Ϊt��������
	tmp = t->right->left;
	t->right->left = t;
	t->right = tmp;
	if(tmp)
		tmp->parent = t;
	//������ڵ㱻��ת���޸ĸ��ڵ�ָ��
	if (root == t)
		root = t->parent;
}
template<class K, class V>
void RBTreeMap<K, V>::right_rotate(RBTreeNode<K, V>* t) {
	//��t��parent��Ϊt->left��parent
	RBTreeNode<K, V>* tmp = t->parent;
	if (t->parent) {
		if (t == t->parent->left) {
			t->parent->left = t->left;
		}
		else {
			t->parent->right = t->left;
		}
	}
	t->parent = t->left;
	t->left->parent = tmp;
	//��t->left����������Ϊt��������
	tmp = t->left->right;
	t->left->right = t;
	t->left = tmp;
	if (tmp)
		tmp->parent = t;
	//������ڵ㱻��ת�ˣ��޸ĸ��ڵ�ָ��
	if (root == t)
		root = t->parent;
}
