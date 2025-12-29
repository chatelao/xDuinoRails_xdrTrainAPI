# Shortest CLI Commands

This document lists the shortest form for each CLI command.

| Command | Ultrashort Form | Shortest Form | Example with Keys | Example with Values |
| :--- | :--- | :--- | :--- | :--- |
| `POWER` | `1` | `<1>` | `<POWER state="ON">` | `1` |
| `POWER` | `0` | `<0>` | `<POWER state="OFF">` | `0` |
| `SET_POWER` | `p` | `<p>` | `<SET_POWER track="1" state="1">` | `p 1 1` |
| `STATUS` | `c` | `<c>` | `<STATUS>` | `c` |
| `FULL_STATUS` | `S` | `<S>` | `<FULL_STATUS>` | `S` |
| `HARDWARE_INFO` | `i` | `<i>` | `<HARDWARE_INFO>` | `i` |
| `REBOOT` | `Z` | `<Z>` | `<REBOOT>` | `Z` |
| `THROTTLE` | `t` | `<t>` | `<THROTTLE cab="123" speed="100" direction="1" steps="128">` | `t 123 100 1 128` |
| `SIMPLE_THROTTLE` | `s` | `<s>` | `<SIMPLE_THROTTLE cab="123" speed="100" direction="1">` | `s 123 100 1` |
| `FUNCTION` | `f` | `<f>` | `<FUNCTION cab="123" function="1" state="1">` | `f 123 1 1` |
| `FUNCTION` | `F` | `<F>` | `<FUNCTION cab="123" function="1" state="0">` | `F 123 1 0` |
| `LIST_LOCOS` | `l` | `<l>` | `<LIST_LOCOS>` | `l` |
| `EMERGENCY_STOP_LOCO` | `X` | `<X>` | `<EMERGENCY_STOP_LOCO cab="123">` | `X 123` |
| `EMERGENCY_STOP_ALL` | `K` | `<K>` | `<EMERGENCY_STOP_ALL>` | `K` |
| `RESET_EMERGENCY_STOP_ALL` | `E` | `<E>` | `<RESET_EMERGENCY_STOP_ALL>` | `E` |
| `DELETE_LOCO` | `R` | `<R>` | `<DELETE_LOCO cab="123">` | `R 123` |
| `READ_CV` | `R` | `<R>` | `<READ_CV cv="1" track="1">` | `R 1 1` |
| `WRITE_CV_BYTE` | `W` | `<W>` | `<WRITE_CV_BYTE cv="1" value="123" track="1">` | `W 1 123 1` |
| `WRITE_CV_BIT` | `B` | `<B>` | `<WRITE_CV_BIT cv="1" bit="7" value="1" track="1">` | `B 1 7 1 1` |
| `PAGED_MODE_WRITE` | `P` | `<P>` | `<PAGED_MODE_WRITE cv="1" value="123">` | `P 1 123` |
| `DIRECT_MODE_WRITE` | `M` | `<M>` | `<DIRECT_MODE_WRITE address="1024" value="123">` | `M 1024 123` |
| `TURNOUT` | `T` | `<T>` | `<TURNOUT id="456" state="1">` | `T 456 1` |
| `ACCESSORY` | `Q` | `<Q>` | `<ACCESSORY id="789" state="1">` | `Q 789 1` |
| `VPIN` | `Y` | `<Y>` | `<VPIN id="1" state="1">` | `Y 1 1` |
| `DEFINE_VPIN` | `N` | `<N>` | `<DEFINE_VPIN vpin="1" type="INPUT" state="0">` | `N 1 INPUT 0` |
| `UNDEFINE_VPIN` | `U` | `<U>` | `<UNDEFINE_VPIN vpin="1">` | `U 1` |
| `JSON_QUERY` | `J` | `<J>` | `<JSON_QUERY type="T">` | `J T` |
| `JSON_CONFIG` | `=>` | `<=>` | `<JSON_CONFIG json="{...}">` | `=> "{...}"` |
| `DIAGNOSTIC` | `D` | `<D>` | `<DIAGNOSTIC command="RAM">` | `D RAM` |
| `LIST_VARIABLES` | `V` | `<V>` | `<LIST_VARIABLES>` | `V` |
| `ECHO` | `+` | `<+>` | `<ECHO state="ON">` | `+` |
| `ECHO` | `-` | `<->` | `<ECHO state="OFF">` | `-` |
| `LIST_COMMANDS` | `?` | `<?>` | `<LIST_COMMANDS>` | `?` |
