
RM := rm -rf
OBJDIR := ./obj

GENERATED_TARGETS := \
./build/linux/classtest.i686 \
./build/linux/classtest.purecov \
./build/linux/classtest.pcv \

INCLUDE_DIRS := \
-I ./src/mgframework \
-I ./src/mgframework/stubs \

OBJS := \
$(OBJDIR)/main.o \
$(OBJDIR)/test_MGAStarNode.o \
$(OBJDIR)/test_MGComponent.o \
$(OBJDIR)/test_MGMap_initialize.o \
$(OBJDIR)/test_MGPathGenerator_calculatePathAStar.o \
$(OBJDIR)/mgclasstester.o \
$(OBJDIR)/mgcomponent.o \
$(OBJDIR)/mgframework.o \
$(OBJDIR)/mgmap.o \
$(OBJDIR)/mgmovingobject.o \
$(OBJDIR)/mgpathgenerator.o \
$(OBJDIR)/mgpathitem.o \
$(OBJDIR)/mgperiodicevent.o \
$(OBJDIR)/mgstationaryobject.o \
$(OBJDIR)/mgsymboltable.o \
$(OBJDIR)/mgwindow.o \
$(OBJDIR)/mgframeworkstub.o \

CC_DEPS := \
$(OBJDIR)/main.d \
$(OBJDIR)/mgframework/class_test/test_MGAStarNode.d \
$(OBJDIR)/mgframework/class_test/test_MGComponent.d \
$(OBJDIR)/mgframework/class_test/test_MGMap_initialize.d \
$(OBJDIR)/mgframework/class_test/test_MGPathGenerator_calculatePathAStar.d \
$(OBJDIR)/mgframework/mgclasstester.d \
$(OBJDIR)/mgframework/mgcomponent.d \
$(OBJDIR)/mgframework/mgframework.d \
$(OBJDIR)/mgframework/mgmap.d \
$(OBJDIR)/mgframework/mgmovingobject.d \
$(OBJDIR)/mgframework/mgpathgenerator.d \
$(OBJDIR)/mgframework/mgpathitem.d \
$(OBJDIR)/mgframework/mgperiodicevent.d \
$(OBJDIR)/mgframework/mgstationaryobject.d \
$(OBJDIR)/mgframework/mgsymboltable.d \
$(OBJDIR)/mgframework/mgwindow.d \
$(OBJDIR)/mgframework/stubs/mgframeworkstub.d \

GCC_C++_FLAGS := -D__GNUC__=4 -DUNITTEST -DUNITTEST_LINUX $(INCLUDE_DIRS) -O0 -g3 -Wall -m32 -fmessage-length=0 -mfpmath=sse -msse2 -fno-builtin -std=c++0x -pedantic -Wall -Wno-long-long -gdwarf-2 -MMD -MP
GCC_C++_BUILD := g++ -c $(GCC_C++_FLAGS)

# purecov
PURECOV_TARGET := ./build/linux/classtest.purecov
PURECOV_VIEW_FILE := ./build/linux/classtest.purecov.pcv
PURECOV_OPTIONS :=-cache-dir=.pure_cache -always-use-cache-dir=yes -linker=ld -g++=yes -max_threads=320 -best-effort -ptr64=no

all: build

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/main.o: ./src/main.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/test_MGAStarNode.o: ./src/mgframework/class_test/test_MGAStarNode.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/test_MGComponent.o: ./src/mgframework/class_test/test_MGComponent.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/test_MGMap_initialize.o: ./src/mgframework/class_test/test_MGMap_initialize.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/test_MGPathGenerator_calculatePathAStar.o: ./src/mgframework/class_test/test_MGPathGenerator_calculatePathAStar.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgclasstester.o: ./src/mgframework/mgclasstester.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgcomponent.o: ./src/mgframework/mgcomponent.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgframework.o: ./src/mgframework/mgframework.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgmap.o: ./src/mgframework/mgmap.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgmovingobject.o: ./src/mgframework/mgmovingobject.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgpathgenerator.o: ./src/mgframework/mgpathgenerator.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgpathitem.o: ./src/mgframework/mgpathitem.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgperiodicevent.o: ./src/mgframework/mgperiodicevent.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgstationaryobject.o: ./src/mgframework/mgstationaryobject.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgsymboltable.o: ./src/mgframework/mgsymboltable.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgwindow.o: ./src/mgframework/mgwindow.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJDIR)/mgframeworkstub.o: ./src/mgframework/stubs/mgframeworkstub.cpp | $(OBJDIR)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(GCC_C++_BUILD) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# regular ---------------

build: ./build/linux/classtest.i686

rebuild: clean ./build/linux/classtest.i686

run: build
	mkdir -p ./build/win/VC/Project2/Release/class_test/result
	./build/linux/classtest.i686 -classtest all > ./build/win/VC/Project2/Release/class_test/result/tc_101_all_testcases.log
	@echo ' '

./build/linux/classtest.i686: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(OBJS) -o "./build/linux/classtest.i686" --verbose -static-libstdc++
	@echo 'Finished building target: $@'
	@echo ' '

# purecov ---------------

purecov: $(PURECOV_TARGET)

purecovrun: $(PURECOV_TARGET)
	$(PURECOV_TARGET) -classtest all

purecovview: purecovrun $(PURECOV_VIEW_FILE)
	@echo Running Purecov on test suite $(PURECOV_TARGET)
	purecov -view $(PURECOV_VIEW_FILE)

$(PURECOV_TARGET): ./build/linux/classtest.i686
	@echo Purecov $(PURECOV_TARGET)
	purecov $(PURECOV_OPTIONS) g++ $(GCC_C++_FLAGS) -lpthread $(OBJS) -static-libstdc++ -o $(PURECOV_TARGET)

# clean ---------------

clean:
	-$(RM) $(OBJS) $(CC_DEPS) $(GENERATED_TARGETS) $(OBJDIR)
	@echo ' '


