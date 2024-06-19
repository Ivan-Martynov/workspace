local function new_account(initial_balance)
    local self = {
        balance = initial_balance,
        limit = 10000.0,
    }
    
    local withdraw = function(v)
        self.balance = self.balance - v
    end

    local deposit = function(v)
        self.balance = self.balance + v
    end

    local extra = function()
        if self.balance > self.limit then
            return self.balance * 0.1
        else
            return 0
        end
    end

    local get_balance = function()
        return self.balance + extra()
    end

    return {
        withdraw = withdraw,
        deposit = deposit,
        get_balance = get_balance,
    }
end

local a1 = new_account(120)
a1.deposit(40)
a1.withdraw(130)
print(a1.get_balance())
