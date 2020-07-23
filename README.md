# HoneyBest LSM
HoneyBest is the new implementation of Linux Security Module project. 
### Background
###### On Linux distribution over the year, few security modules have been developed, such as SELinux / Apparmor / Smack / Tomoyo project, but there is still huge space to make improvement nevertheless. Most of the Linux user keep apart from existing security modules mainly because it make a high entry barrier for those who have little understanding of system behavior & rules. In order to build the module more friendly, our target is to hide the complexity of making rules, but also allow advanced user to be able to refine the granularity. 
###### For most of the case, security module begin to involve in post software development. Take an embedded devices, NAS appliance for the example. Developer have to write a bunch of rules to protect applications, configuration files from other unauthorized process & restriction to certain resources. In order to do so, developer had to go deep through every single process to generating rules. Here we start to ask ourselves few question, is there any possible we can build an auto generation secure module policy base on existent scenario? How if the secure module policy support interaction with developer whether or not to add new rules or permission under safe condition? Can user choose different granularity level based on their condition? Those questions are the reason why we build HoneyBest secure module.
### Concept
###### Let us imaging few conditions here. 
##### Condition A – Environment complexity is hard to apply rules
###### Team of developers have complete their software development on Linux box. The appliance involve NGIX server for user to configure setting; Samba server for file sharing; SNMP server for remote setting; Syslog server to track system record. They handle the appliance to one of their security guy, Bob, who are the expertise in security module. In order to create thread model, Bob have to understand every single process running on the box, how each process interfere with system and other processes. He now create rules to protect base on the thread model. At first, he create rules to restrict process to access only certain system resource, such as Syslog server. Syslog server is allowed to create files under /var/log/*.log, with WRITE permission only; Syslog server is allow to create only localhost 514 UDP port, receive other application log message. Here come small part of complicate scenario, log message files could grow up over the time and Logrotate daemon are design into system to handle compression job; log message files need permission rules to MOVE files(DELETE/CREATE/READ/WRITE); Meanwhile, NGIX we server need permission READ in order to show context while user login via web page. After Bob figure out all those cross over relationship & rules, he start to apply into system. It turn out, the box does not act as normal as expect to pass system integration test. Bob have to invite developer to figure out what going on to the system. It turn out that NGIX web server need permission rules to interact with 514 UDP port for logging itself message. In real world, security expertise feel frustrate to do their job because of complexity environment involve.
##### Condition B – High learning curves
###### User, roles, level, category, labeling, and hats are not easy to understand, those are security expertise concept with specific tools. Most of the small/medium company do not have security expertise to rely on. We want to help software developers secure their product as much as we can.
##### Condition C – Untrusted root in design
###### The complete security policies should treat super user (root) as normal root. Root are not allow to change others policies but its own. Penetration to root user might corrupt whole policies wall you made. In our design, policies update or change should bind tightly with secure boot process, more precisely, with hardware Root of Trust.
##### Condition D – Interaction in real time instead of post rules applied
###### Real time interaction feedback mechanism are more easy way for developers to understand what going. Instead of rules, pop out dialogue asking permission to add rule is an effective way to make progress. For the fine-grain advanced user, our design also consider to fulfill such needs.
##### Condition E – I want my program to be protected from other process, user or even root.
###### In some privacy scenario, system designer not only require the task to have restriction from accessing resources, but also restriction from other resources to access the task. Here are the 2 examples, I want to protect my private libraries/program from piracy, however, still allow certain program to use; I want only “upgradefirmware” command to be able upgrade system firmware, not “dd” command, and the integrity of “upgradefirmware” command is concerned. 
### Design
###### Our core design is to focus on capturing the kernel activities triggered by user space program. Activities which is tracking will later turn into list data structure for security module to detect an unexpected occur event. The size of list data structure is tightly depends on level of granularity. The more precise restriction or control to be chosen, the higher space requirement for data structure to be saved. Above the surface of such design, here is the approach to apply secure module. Unfreeze the box in your security environment, run all activities as you can to create a model, then freeze the box. Once you freeze the box, all activities are restrict to previous model. You might consider fine-grain the model because some activities are not able to perform in your security environment. Either user editor to edit the model or turn on interaction mode, developers are able to selectively set policies in real world situation. Below figure show how the lifecycle go:
1.	Product finish development
2.	Turn on unfreeze mode / Turn off interaction mode
3.	1st End to End System Integration Test 
4.	Turn off unfreeze mode / Turn on interaction mode
5.	2nd End to End System Integration Test or Manually edit model
6.	Turn off interaction mode.

