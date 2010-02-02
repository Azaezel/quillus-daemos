require("common.Utilities")

StateMachine = {}

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function initStateMachine(stateMachineName)
	local stateMachine = create(StateMachine)

    stateMachine.name = stateMachineName
	stateMachine.states = {};
    stateMachine.stack = {};

    -- Make currentState be private
    local currentState;

    
    -- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    -- Get the state machine on the stop of the stack
    stateMachine.getStackTop = function(self)
        if table.getn(self.stack) == 1 then
            return self;
        end
    end

    stateMachine.getCurrentState =  function(self)
        -- If the stack is a size of 1, then
        -- this machine is the only machine on the stack
        if table.getn(self.stack) == 1 then
            return currentState;
        end

        if table.getn(self.stack) == 0 then
            print("Error, stack underflow");
            return nil;
        end

        -- Otherwise, recurse
        return self.stack[table.getn(self.stack)]:getCurrentState();
    end

    stateMachine.setCurrentState = function(self, newState)
        -- If the stack is a size of 1, then
        -- this machine is the only machine on the stack
        if table.getn(self.stack) == 1 then
            currentState = newState
        else
            -- Otherwise, recurse
            self.stack[table.getn(self.stack)].setCurrentState(state);
        end
    end

    stateMachine:doCall(stateMachine);
	return stateMachine;
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:newState(name)
    local state = 
    {
        name = name,
        machine = self,
        eventHandlers = {},
        onTransition = {},
        onEnter = function() end,
        onExit = function() end
    }

    state.handle = function(self, name, handlerFunction)
        self.eventHandlers[name] = handlerFunction;
        return self;
    end

    state.transition = function(self, name, handlerFunction)
        self.onTransition[name] = handlerFunction;
        return self;
    end
    
    self.states[name] = state;
    
    return state;
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:init()
	self:transitionTo("init")
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:getState(stateName)
	return self.states[stateName]
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:fireEvent(eventName, ...)
	if self:getCurrentState() == nil then
		print("Illegal transition to event " .. eventName)
		return
	end    
	if self:getCurrentState().eventHandlers ~= nil then
		local eventHandler = self:getCurrentState().eventHandlers[eventName]
		if eventHandler ~= nil then
			eventHandler(self:getCurrentState(), unpack(arg))
		else
			-- Ignore invalid events
		end
	else
		print("State machine is in an invalid state")
	end
end


-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:handleTransition()
    -- First, exit the existing state
    local currentState = self:getStackTop():getCurrentState();

	-- Fire onTransition and onExit if the currentState ~= nil
	if currentState ~= nil then
		-- Find  transition event
		if currentState.onTransition ~= nil then
			if currentState.onTransition[stateName] ~= nil then
				-- Fire the transition event
				currentState.onTransition[stateName](currentState)
			end
		end

		-- Fire the  onExit event
		if currentState.onExit ~= nil then
			currentState.onExit(currentState)
		end
	end
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:doCall(stateMachine)
    -- Push the current state onto the stack    
    self.stack[table.getn(self.stack) + 1] = stateMachine;
    stateMachine.parent = self;
    stateMachine:init();
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:doReturn()
    self:handleTransition();

    -- Set the top of the stack to nil
    self.parent.stack[table.getn(self.parent.stack)] = nil

    -- Decrease the stack size by 1
    --table.setn(self.stack, table.getn(self.stack) - 1);

    -- Call the onReturn
    if self.parent:getCurrentState().onReturn ~= nil then
        self.parent:getCurrentState():onReturn()
    end
end

-- ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
function StateMachine:transitionTo(stateName)
	if self:getState(stateName) == nil then
		print("Invalid state " .. stateName)
		return
	end

    -- Get the current state and the next state
    local currentState = self:getStackTop():getCurrentState();
    local nextState = self:getStackTop():getState(stateName);

    self:handleTransition();

    print("State machine " .. self.name .. " entering state " .. stateName)
	-- Change the current state
	self:setCurrentState(nextState);

	-- Fire the onEnter event
	if nextState.onEnter ~= nil then
		nextState:onEnter(currentState)
	end	
end
