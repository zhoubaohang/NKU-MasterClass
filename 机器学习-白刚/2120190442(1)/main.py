from models import FindS, CandidateElimination

data = [
    ['Sunny', 'Hot', 'High', 'False', 'No'],
    ['Sunny', 'Hot', 'High', 'True', 'No'],
    ['Overcast', 'Hot', 'High', 'False', 'Yes'],
    ['Rain', 'Mild', 'High', 'False', 'Yes'],
    ['Rain', 'Cool', 'Normal', 'False', 'Yes'],
    ['Rain', 'Cool', 'Normal', 'True', 'No'],
    ['Overcast', 'Cool', 'Normal', 'True', 'Yes'],
    ['Sunny', 'Mild', 'High', 'False', 'No'],
    ['Sunny', 'Cool', 'Normal', 'False', 'Yes'],
    ['Rain', 'Mild', 'Normal', 'False', 'Yes'],
    ['Sunny', 'Mild', 'Normal', 'True', 'Yes'],
    ['Overcast', 'Mild', 'High', 'True', 'Yes'],
    ['Overcast', 'Hot', 'Normal', 'False', 'Yes'],
    ['Rain', 'Mild', 'High', 'True', 'No']
]

# data = [
#     ['Rainy', 'Cold', 'High', 'Strong', 'Warm', 'Change', 'No'],
#     ['Sunny', 'Warm', 'Normal', 'Strong', 'Warm', 'Same', 'Yes'],
#     ['Sunny', 'Warm', 'High', 'Strong', 'Warm', 'Same', 'Yes'],
#     ['Sunny', 'Warm', 'High', 'Strong', 'Cool', 'Change', 'Yes']
# ]

finds_result = FindS(data)
print(f"Find-S算法结果\n{finds_result}\n\n")

ce = CandidateElimination(data)
ce()
print(f"候选消除法结果\n特殊边界:{ce.S}\n一般边界:{ce.G}")