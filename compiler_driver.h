class compiler_driver{
public:
	compiler_driver();
	virtual ~compiler_driver();

	std::map<std::string, int> variables;

	int result;
	void scan_begin();
	void scan_end();
	bool trace_scanning;
}
