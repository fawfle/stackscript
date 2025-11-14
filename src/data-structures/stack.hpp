#define GROWTH_FACTOR 2
#define MIN_CAPACITY 10

class Stack {
	private:
		int *array = new int[MIN_CAPACITY];
		int capacity = MIN_CAPACITY;
		int size = 0;
		
		void resize_array();

	public:
		void push(int val);
		int pop();

		int peek() const;
		bool is_empty() const { return size == 0; };

		~Stack();
};
