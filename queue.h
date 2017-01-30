int queue_get_direction(int previous_floor,int previous_direction);
int queue_should_stop(int floor, int direction);
void queue_clear_orders_at_floor(int floor);
void queue_remove_all_orders();
void queue_add_orders(int floor,int button);
