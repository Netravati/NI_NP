/*
Author: NP
NI-Berlin Code Challenge
*/

#include<memory>
#include<cassert>
#include<iostream>

// AudioEffect is the base class for effects that can process 
// audio and have a subsequent effect (next).

struct AudioEffect 
{
	virtual ~AudioEffect() = default;
	virtual void process(float* buf, size_t num) = 0;
	std::shared_ptr<AudioEffect> next;
};//AudioEffect can be improved: Answer is in Readme file

// Effect struct derived from AudioEffect to define virtual method and struct.
struct Effect : AudioEffect 
{
public:
	Effect(){next = nullptr;} 
	~Effect(){}
	void process(float* buf, size_t num)
	{
	    //process the audio wrt requirements, which is not necessary with this assignment for now.
	}
};

//We can further add different audio effect processing according to
//the requirements like Noise gate, Gain or compressor for sample example as below.

//Noise Gate is a audio effect: to decide on making audio silent or not according to threshold.
struct NoiseGate: public AudioEffect
{
    float threshold;
    void process(float *buf, size_t num)
    {
        if (*buf > threshold)
            *buf = threshold;
    }
};

//Gain Boost: is calculated as a ratio of output power to input power to add for audio effect.
struct GainBoost: public AudioEffect
{
    float ratio;
    void process(float *buf, size_t num)
    {
        *buf = *buf + ratio;
    }
};

//Compressor: value to compress for audio.
struct Compressor: public AudioEffect
{
    float offset;
    void process(float *buf, size_t num)
    {
        *buf = *buf - offset;
    }
};

// Using Floyd’s Cycle-Finding Algorithm to detect loop.
// source: https://en.wikipedia.org/wiki/Cycle_detection  & https://tinyurl.com/vxc67ua3
//
// detect_feedback gets std::shared_ptr<AudioEffect> as input parameter and returns boolean value.
// Function returns 'true' if there is a loop detection & 'false' in either case.
bool detect_feedback(std::shared_ptr<AudioEffect> ptr)
{
    std::shared_ptr<AudioEffect> slow_p = ptr, fast_p = ptr;
    while (slow_p && fast_p && fast_p->next) {
        slow_p = slow_p->next;
        fast_p = fast_p->next->next;
        if (slow_p == fast_p) {
            return 1;
        }
    }
    return 0;
}
void test_no_feedback()
{
	std::cout << "Testing a list with no feedback.." << std::endl;
	auto head = std::make_shared<Effect>();
	auto ae1 = std::make_shared<Effect>();
	auto ae2 = std::make_shared<Effect>();
	auto ae3 = std::make_shared<Effect>();
	
	head->next = ae1;
	ae1->next = ae2;
	ae2->next = ae3;

	bool feedback = detect_feedback(head);
	assert(feedback == false);
	std::cout << std::boolalpha << "\t" << "Expected: false Feedback detected: " << feedback << std::endl; 
}


// When ownership of ae1 is stored with another shared pointer (ae3 which causes the loop)
// a cyclical reference causes a memory leak (ie.e the Effect is only deleted when all
// the ->next references are deleted but these are only deleted once the Effect is deleted.)
//
// When two shared_ptrs reference eachother, the reference count will never reach zero 
// and so will never be deleted / memory never be freed.
void test_with_feedback()
{
	std::cout << "Testing a list with feedback.." << std::endl;
	auto head = std::make_shared<Effect>();
	auto ae1 = std::make_shared<Effect>();
	auto ae2 = std::make_shared<Effect>();
	auto ae3 = std::make_shared<Effect>();
	
	head->next = ae1;
	ae1->next = ae2;
	ae2->next = ae3;
	ae3->next = ae1; // Causes the cyclical reference / memory leak
	
	bool feedback = detect_feedback(head);
	assert(feedback == true);
	std::cout << std::boolalpha << "\t" << "Expected: true Feedback detected: " << feedback << std::endl; 
}



int main(void) 
{
    test_with_feedback();
    test_no_feedback();
}

/*
Note: Answer for Question 2 is in Read Me file!
*/
