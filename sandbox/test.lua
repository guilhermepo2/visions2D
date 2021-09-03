function BeginPlay()
    _Log("lua BeginPlay()")
end

function Update(self, DeltaTime)
    _Move(self, 50 * DeltaTime, 0)
end