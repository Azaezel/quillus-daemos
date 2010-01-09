
SplashScreen = {}

-- Create a sub-state machine
function SplashScreen:init(splashSequence)
    -- Create the sub-state machine
    local subStateMachine = initStateMachine(splashSequence.stateMachine.name .. ":" .. splashSequence.stateName);

    -- Create the state for handling this sequence
    local state = splashSequence.stateMachine:newState(splashSequence.stateName);

    -- When the state is entered, call the sub state machine. 
    state.onEnter = function(state)
        state.machine:doCall(subStateMachine);
    end

    -- When the sub state machine returns, transition to the next state
    state.onReturn = function(state)
        print("Doing on return");
        state.machine:transitionTo(splashSequence.nextState);
    end

    -- Create the init state for the sub-state machine
    local init = subStateMachine:newState("init");

    if table.getn(splashSequence.sequence) > 0 then
        -- On initial entry, just transition to the first screen
        init.onEnter = function(state)
            state.machine:transitionTo("screen1");
        end

        for index, splashItem in ipairs(splashSequence.sequence) do
            print("Creating new splash screen state: " .. "screen" .. index);
 
            -- Create a new state for this splash screen
            local screenState = subStateMachine:newState("screen" .. index);

            -- Save the splash item as part of the state
            screenState.splashItem = splashItem;
            screenState.splashItem.index = index;
            screenState.splashSequence = splashSequence;

            -- Set the time length
            screenState.duration = splashItem.duration;
            screenState.nextState = "screen" .. index + 1

            -- Reset the timeLeft value when the state is entered.
            -- Not that it's likely to happen, but this will improve
            -- reuse of the sequence.
            screenState.onEnter = function(state)
                state.timeLeft = screenState.duration;
                state.timeUsed = 0;

                if splashSequence.showMethod ~= nil then
                    splashSequence.showMethod(self, splashItem);
                end
            end
            
            screenState.onExit = function(state)
                if splashSequence.hideMethod ~= nil then
                    splashSequence.hideMethod(self, splashItem);
                end
            end

            screenState:handle("clockTick", function(state, timeDelta)
                state.timeLeft = state.timeLeft - timeDelta;
                state.timeUsed = state.timeUsed + timeDelta;
                
                if screenState.timeLeft < 0 then
                    -- Transition to the next state, or if there's not one
                    -- then return.
                    if (state.machine:getState(screenState.nextState) ~= nil) then
                        state.machine:transitionTo(screenState.nextState);
                    else
                        state.machine:doReturn();
                    end
                else
                    splashSequence.onTick(state);
                end
            end);
        end
    else
        -- Sequence is empty, so return immediately
        init.onEnter = function(state, previousState)
            state.machine:doReturn();
        end
    end

	return o
end

