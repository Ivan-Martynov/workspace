import { Schema, model } from 'mongoose'

const userSchema = new Schema(
  {
    username: {
      type: String,
      unique: true,
      required: true,
      minLength: 3,
      maxLength: 30,
    },
    email: {
      type: String,
      unique: true,
      required: true,
    },
    passwordHash: { type: String, required: true },
    role: { type: String, enum: ['user', 'admin'], default: 'user' },
    verified: { type: Boolean, default: false },
    verificationToken: { type: String, default: null, index: { sparse: true } },
    verificationTokenExpiresAt: { type: Date, default: null },
    resetToken: { type: String, default: null, index: { sparse: true } },
    resetTokenExpiresAt: { type: Date, default: null },
  },
  { timestamps: true },
)

userSchema.set('toJSON', {
  transform: (_document, returnedObject) => {
    returnedObject.id = returnedObject._id.toString()
    delete returnedObject.passwordHash
    delete returnedObject.verificationToken
    delete returnedObject.verificationTokenExpiresAt
    delete returnedObject.resetToken
    delete returnedObject.resetTokenExpiresAt
    delete returnedObject._id
    delete returnedObject.__v
  },
})

export default model('User', userSchema)
