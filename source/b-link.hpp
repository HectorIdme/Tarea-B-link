// Copyright
#ifndef SOURCE_B_LINK_HPP_
#define SOURCE_B_LINK_HPP_

#include <utility>
#include <set>
#include <iostream>
#include <queue>

namespace EDA {
namespace Concurrent {


template<typename T>
struct NodeBLink;

template<typename T>
struct Node {
	T value;
	NodeBLink<T>* l;
	NodeBLink<T>* r;

	Node(T val, NodeBLink<T>* l_ = NULL, NodeBLink<T>* r_ = NULL) {
		value = val;
		l = l_;
		r = r_;
	}

	int getVal() const { return value; }

	NodeBLink<T>* getL() const { return l; }

	NodeBLink<T>* getR() const { return r; }


	void setL(NodeBLink<T> x)const {
		l = &x;
	}
	void setR(NodeBLink<T> x)const {
		r = &x;
	}

};

template<typename T>
struct classcomp {
	bool operator() (Node<T> a, Node<T> b)const
	{
		return a.value < b.value;
	}
};

template<typename T>
struct NodeBLink {
	std::set<Node<T>, classcomp<T> > valores;
	NodeBLink<T>* parent;
	std::set<Node<T>, classcomp<T> >* left;
	std::set<Node<T>, classcomp<T> >* right;

	NodeBLink() { left = right = NULL;  parent = NULL; }
};

template <std::size_t B, typename Type>
class BLinkTree {
 public:

	typedef Type data_type;

	BLinkTree() { root = NULL; }

	~BLinkTree() {}

	std::size_t size() const {

		size_t tam = 0;

		if (!root) { return 0; }
		else {

			NodeBLink<data_type>* viajero = root;

			std::queue<NodeBLink<data_type>*> cola;
			cola.push(viajero);
			bool colaP = 1;

			while (!cola.empty()) {
				NodeBLink<data_type>* temp = cola.front();
				auto iter = temp->valores.begin();

				if(iter->l == NULL){
					viajero = temp;
					
					while(viajero->right){
						tam += viajero->valores.size();
						viajero = viajero->right;
					}
					tam += viajero->valores.size();

					cola.pop();
				}
				else{
					cola.push(iter->l);
					cola.pop();
				}
			}
		}

		return tam;
	}


	bool empty() const {
		
		if (!root) { return 1; }
		return 0;
	}

	bool search(const data_type& value) const {

		if (!root) { return 0; }
		else {

			NodeBLink<data_type>* viajero = root;

			std::queue<NodeBLink<data_type>*> cola;
			cola.push(viajero);
			bool colaP = 1;

			while (!cola.empty() && colaP) {
				NodeBLink<data_type>* temp = cola.front();
				auto iter = temp->valores.begin();

				for (; iter != temp->valores.end(); iter++) {
					if (iter->value > value) {
						if (iter->l == NULL) {
							viajero = temp;
							colaP = 0;
							cola.pop();
							break;
						}
						else {
							cola.push(iter->l);
							cola.pop();
							break;
						}
					}
				}
				if (iter == temp->valores.end()) {
					iter--;
					if (iter->r == NULL) {
						viajero = temp;
						colaP = 0;
					}
					else {
						cola.push(iter->r);
						cola.pop();
					}
				}

			}
			
			bool res = viajero->valores.count(value);
			if (res) { std::cout << "encontrado valor: " << value << "\n"; return 1; }
			else { std::cout << "no encontrado valor: " << value << "\n"; return 0; }

		}
	}


	bool insert(const data_type& value) {

		int medio = B / 2;

		if (!root) {
			root = new NodeBLink<data_type>();
			Node<data_type> nuevo(value);
			root->valores.insert(nuevo);

		}
		else {

			//bool encontrado = search(value);
			//if (encontrado) { std::cout << "Ya insertado valor: "<<value<<"\n"; return 0; }

			NodeBLink<data_type>* viajero = root;

			std::queue<NodeBLink<data_type>*> cola;
			cola.push(viajero);
			bool colaP = 1;

			while (!cola.empty() && colaP) {
				NodeBLink<data_type>* temp = cola.front();
				auto iter = temp->valores.begin();
		
				for (; iter != temp->valores.end(); iter++) {
					if (iter->value > value) {
						if (iter->l == NULL) {
							viajero = temp;
							colaP = 0;
							cola.pop();
							break;
						}
						else {
							cola.push(iter->l);
							cola.pop();
							break;
						}
					}
				}
				if (iter == temp->valores.end()) {
					iter--;
					if (iter->r == NULL) {
						viajero = temp;
						colaP = 0;
					}
					else {
						cola.push(iter->r);
						cola.pop();
					}
				}
		
			}



			Node<data_type> nuevo(value);
			viajero->valores.insert(nuevo);

			int cont = 0;

			while (viajero->valores.size() >= B) {

				NodeBLink<data_type>* prev = new NodeBLink<data_type>();

				int contador_medio = 0;
				while (contador_medio < medio) {
					auto it = viajero->valores.begin();
					Node<data_type> copia(it->getVal(), it->getL(), it->getR());
					prev->valores.insert(copia);

					if (cont > 0) {
						it->getL()->parent = it->getR()->parent = prev;
					}

					viajero->valores.erase(it->value);
					contador_medio++;

				}

				int valorM = 0;
				auto valu = viajero->valores.begin();
				valorM = valu->value;
				if (cont > 0) {
					auto it = viajero->valores.begin();
					viajero->valores.erase(it->value);
				}

				if (viajero->parent == NULL) {
					NodeBLink<data_type>* nueva_raiz = new NodeBLink<data_type>();
					Node<data_type> nodo_nueva_raiz(valorM, prev, viajero);
					nueva_raiz->valores.insert(nodo_nueva_raiz);
					prev->parent = viajero->parent = nueva_raiz;
					this->root = nueva_raiz;

					prev->right = viajero;
					if(viajero->left == NULL){
						viajero->left = prev;
					}
					else{
						prev->left = viajero->left;
						viajero->left->right = prev;
						viajero->left = prev;
					}

				}
				else {
					NodeBLink<data_type>* parentTemp = viajero->parent;
					Node<data_type> nodo_nueva_padre(valorM, prev, viajero);
					parentTemp->valores.insert(nodo_nueva_padre);

                    NodeBLink<data_type>* viajero_aux = viajero;
                    NodeBLink<data_type>* prev_aux = prev;

                    prev_aux->parent = viajero_aux->parent = parentTemp;

					auto iterR = parentTemp->valores.begin();
					if (iterR->value == valorM) {
						auto tempIR = iterR;
						tempIR++;
						Node<data_type> nodo_nuevo(tempIR->getVal(), viajero_aux, tempIR->getR());
						parentTemp->valores.erase(tempIR->getVal());
						parentTemp->valores.insert(nodo_nuevo);

                        //tempIR->getR()->parent = viajero_aux->parent = parentTemp;
					}
					else {
						for (; iterR != parentTemp->valores.end(); iterR++) {
							auto tempIR = iterR;
							tempIR++;
							if (tempIR->value == valorM) {
								Node<data_type> nodo_nuevo(iterR->getVal(), iterR->getL(), prev_aux);
								parentTemp->valores.erase(iterR->getVal());
								parentTemp->valores.insert(nodo_nuevo);

                                //iterR->getL()->parent = prev_aux->parent = parentTemp;
								break;
							}
						}

						iterR = parentTemp->valores.begin();
						for (; iterR->value != valorM; iterR++) {}
						iterR++;
                        viajero_aux = viajero;

						if (iterR != parentTemp->valores.end()) {
							Node<data_type> nodo_nuevo(iterR->getVal(), viajero_aux, iterR->getR());
							parentTemp->valores.erase(iterR->getVal());
							parentTemp->valores.insert(nodo_nuevo);

                            //iterR->getR()->parent = viajero_aux->parent = parentTemp;
						}
					}

					prev->right = viajero;
					if(viajero->left == NULL){
						viajero->left = prev;
					}
					else{
						prev->left = viajero->left;
						viajero->left->right = prev;
						viajero->left = prev;
					}


					viajero = viajero->parent;

				}
				cont++;

			}
		}std::cout << "Insertado valor: " << value << "\n";
		return 1;
	}

	void remove(const data_type& value) {}

 private:
	NodeBLink<data_type>* root;

};

}  // namespace Concurrent
}  // namespace EDA

#endif  // SOURCE_B_LINK_HPP_
