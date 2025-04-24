#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_
class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();
	void start();
	void stop();
	void paused();
	void unpaused();
	int get_ticks();
	bool is_paused();
	bool is_started();
	int get_start_tick() { return start_tick_; }
	void reset_time(){ start_tick_ = 0; }
private:
	int start_tick_;
	int paused_tick_;

	bool is_paused_;
	bool is_started_;
  };





#endif // !IMP_TIMER_H_
